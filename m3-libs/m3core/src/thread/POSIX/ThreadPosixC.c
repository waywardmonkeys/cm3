/* Copyright (C) 1989, Digital Equipment Corporation           */
/* All rights reserved.                                        */
/* See the file COPYRIGHT for a full description.              */

/* This is based on RTThread.m3, which is platform specific
 * and varied a lot. Some versions cached the mask to use
 * in allow/disallow, some recomputed it each time.
 * Some used sigaction(), some used signal().
 * The users of sigaction() vary as to which flags they use.
 * Some use BSD sigvec which is similar to sigaction.
 */

/* _XOPEN_SOURCE is required to get the correct MacOSX/x86
 * ucontext_t declaration. Otherwise getcontext overruns
 * its parameter.
 */
#define _XOPEN_SOURCE 500
#define _BSD_SOURCE
#define _XPG4_2
#define _DARWIN_C_SOURCE

#if (defined(__APPLE__) && defined(__x86_64__)) || defined(__OpenBSD__)
/* http://www.opengroup.org/onlinepubs/009695399/functions/swapcontext.html
 * http://www.engelschall.com/pw/usenix/2000/pmt-html/
 * Sigaltstack is more portable -- OpenBSD and Darwin/AMD64 do not
 * implement get/set/make/swapcontext.
 */
#define M3_USE_SIGALTSTACK
#endif

#include "m3core.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <setjmp.h>
#include <stddef.h>
#include <errno.h>
#ifndef __OpenBSD__
#include <ucontext.h>
#endif
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*SignalHandler1)(int signo);

#define setup_sigvtalrm     ThreadPosix__setup_sigvtalrm
#define allow_sigvtalrm     ThreadPosix__allow_sigvtalrm
#define disallow_sigvtalrm  ThreadPosix__disallow_sigvtalrm
#define MakeContext         ThreadPosix__MakeContext
#define SwapContext         ThreadPosix__SwapContext
#define DisposeContext      ThreadPosix__DisposeContext
#define ProcessContext      ThreadPosix__ProcessContext
#define InitC               ThreadPosix__InitC

static sigset_t ThreadSwitchSignal;

#ifdef __CYGWIN__
#define SIG_TIMESLICE SIGALRM
#else
#define SIG_TIMESLICE SIGVTALRM
#endif

void
__cdecl
setup_sigvtalrm(SignalHandler1 handler)
{
  struct sigaction act;

  ZERO_MEMORY(act);

  sigemptyset(&ThreadSwitchSignal);
  sigaddset(&ThreadSwitchSignal, SIG_TIMESLICE);

  act.sa_handler = handler;
  act.sa_flags = SA_RESTART;
  sigemptyset(&(act.sa_mask));
  if (sigaction (SIG_TIMESLICE, &act, NULL)) abort();
}

void
__cdecl
allow_sigvtalrm(void)
{
    int i = sigprocmask(SIG_UNBLOCK, &ThreadSwitchSignal, NULL);
    assert(i == 0);
}

void
__cdecl
disallow_sigvtalrm(void)
{
    int i = sigprocmask(SIG_BLOCK, &ThreadSwitchSignal, NULL);
    assert(i == 0);
}

typedef struct {
  void *stackaddr;
  size_t stacksize;
  void *sp;
#ifdef M3_USE_SIGALTSTACK
  sigjmp_buf jb;
#else
  ucontext_t uc;
#endif
} Context;

#ifdef M3_USE_SIGALTSTACK

#define xMakeContext ThreadPosix__xMakeContext

#define SWAP_CONTEXT(oldc, newc)       \
do {                                   \
    if (sigsetjmp((oldc)->jb, 1) == 0) \
        siglongjmp((newc)->jb, 1);     \
} while (0)

static Context mctx_caller;
static sig_atomic_t volatile mctx_called;
static Context * volatile mctx_create;
static void (* volatile mctx_create_func)(void);
static sigset_t mctx_create_sigs;

static void mctx_create_boot(void)
{
    void (*volatile mctx_start_func)(void);
    
    sigprocmask(SIG_SETMASK, &mctx_create_sigs, NULL);
    mctx_start_func = mctx_create_func;
    SWAP_CONTEXT(mctx_create, &mctx_caller);
    mctx_start_func();
    abort(); /* not reached */
}

static void mctx_create_trampoline(int sig)
{
    if (sigsetjmp(mctx_create->jb, 0) == 0)
    {
        mctx_called = 1;
        return;
    }
    
    mctx_create_boot();
}

void
__cdecl
xMakeContext( 
    Context *context, 
    void (*function)(void),
    void *stack,
    size_t stack_size) 
{
    struct sigaction sa;
    struct sigaction osa;
    stack_t ss;
    stack_t oss;
    sigset_t osigs;
    sigset_t sigs;

    ZERO_MEMORY(sa);
    ZERO_MEMORY(osa);
    ZERO_MEMORY(ss);
    ZERO_MEMORY(oss);
    ZERO_MEMORY(osigs);
    ZERO_MEMORY(sigs);

    sigemptyset(&sigs);
    sigaddset(&sigs, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigs, &osigs);
    
    sa.sa_handler = mctx_create_trampoline;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, &osa);
    
    ss.ss_sp = stack;
    ss.ss_size = stack_size;
    ss.ss_flags = 0;
    sigaltstack(&ss, &oss);
    
    mctx_create = context;
    mctx_create_func = function;
    mctx_create_sigs = osigs;
    mctx_called = 0;
    kill(getpid(), SIGUSR1);
    sigfillset(&sigs);
    sigdelset(&sigs, SIGUSR1);
    while (!mctx_called)
        sigsuspend(&sigs);
        
    sigaltstack(NULL, &ss);
    ss.ss_flags = SS_DISABLE;
    sigaltstack(&ss, NULL);
    if (!(oss.ss_flags & SS_DISABLE))
        sigaltstack(&oss, NULL);
    sigaction(SIGUSR1, &osa, NULL);
    sigprocmask(SIG_SETMASK, &osigs, NULL);
    
    SWAP_CONTEXT(&mctx_caller, context);
}

#endif /* M3_USE_SIGALTSTACK */

void *
__cdecl
MakeContext (void (*p)(void), int words)
{
  Context *c = (Context *)calloc (1, sizeof(*c));
  size_t size = words * sizeof(void *);
  int pagesize = getpagesize();
  char *sp = NULL;
  int pages;
  int er;

  if (c == NULL)
    goto Error;
  if (size <= 0) return c;
  if (size < MINSIGSTKSZ) size = MINSIGSTKSZ;

  /* Round up to a whole number of pages, and
   * allocate two extra pages, one at the start
   * and one at the end, and don't allow accessing
   * either one (catch stack overflow and underflow).
   */
  pages = (size + pagesize - 1) / pagesize + 2;
  size = pages * pagesize;
  sp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (sp == NULL)
    goto Error;
  c->stackaddr = sp;
  c->stacksize = size;
  if (mprotect(sp, pagesize, PROT_NONE)) abort();
  if (mprotect(sp + size - pagesize, pagesize, PROT_NONE)) abort();

#ifdef M3_USE_SIGALTSTACK
  xMakeContext(c, p, sp + pagesize, size - 2 * pagesize);
#else
  if (getcontext(&(c->uc))) abort();
  c->uc.uc_stack.ss_sp = sp + pagesize;
  c->uc.uc_stack.ss_size = size - 2 * pagesize;
  c->uc.uc_link = 0;
  makecontext(&(c->uc), p, 0);
#endif /* M3_USE_SIGALTSTACK */

  return c;
Error:
  er = errno;
  if (c != NULL) free(c);
  if (sp != NULL) munmap(sp, size);
  errno = er;
  return NULL;
}

void
__cdecl
SwapContext (Context *from, Context *to)
{
#ifdef M3_USE_SIGALTSTACK
  SWAP_CONTEXT(from, to);
#else
  if (swapcontext(&(from->uc), &(to->uc))) abort();
#endif
}

void
__cdecl
DisposeContext (Context **c)
{
  if (munmap((*c)->stackaddr, (*c)->stacksize)) abort();
  free(*c);
  *c = NULL;
}

void
__cdecl
ProcessContext(Context *c, char *bottom, char *top,
               void (*p) (void *start, void *limit))
{
  size_t xx;
  if (top == NULL) {
    /* live thread */
    /* do we need to flush register windows too? */
#ifdef M3_USE_SIGALTSTACK
    sigsetjmp(c->jb, 0);
#else
    if (getcontext(&(c->uc))) abort();
#endif
    top = (char *)&xx;
  }
  if (bottom < top)
    p(bottom, top);
  else
    p(top, bottom);
#if defined(__APPLE__) && !defined(M3_USE_SIGALTSTACK)
  p(&(c->uc.uc_mcontext[0]), &(c->uc.uc_mcontext[1]));
#else
  p(&c[0], &c[1]);
#endif
}

int
__cdecl
ThreadPosix__SetVirtualTimer(void)
{
    struct timeval selected_interval;
    struct itimerval it;

    ZERO_MEMORY(selected_interval);
    ZERO_MEMORY(it);
    selected_interval.tv_sec = 0;
    selected_interval.tv_usec = 100 * 1000;
    it.it_interval = selected_interval;
    it.it_value    = selected_interval;
    return setitimer(ITIMER_VIRTUAL, &it, NULL);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
