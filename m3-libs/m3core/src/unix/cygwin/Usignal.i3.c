/* $Id$ */

#include <signal.h>
#include <stdio.h>

/*
search for ^\(" *\)\(\:i\)\( *= *\)\(\:n\)\([^"]+", \)
replace with \1\2\3%u\5\1
*/

int main()
{
    unsigned i;
    const static struct
    {
        const char* Format;
        unsigned Value;
    } Data[] =
{
"(* Copyright (C) 1990, Digital Equipment Corporation.                 *)", 0,
"(* All rights reserved.                                               *)", 0,
"(* See the file COPYRIGHT for a full description.                     *)", 0,
"(*                                                                    *)", 0,
"(* Last modified on Mon Jan  5 11:11:07 GMT 1998 by rrw               *)", 0,
"(*      modified on Fri Feb 24 15:18:21 PST 1995 by kalsow            *)", 0,
"(*      modified on Tue Feb 14 20:58:12 GMT 1995 by rrw1000@cam.ac.uk *)", 0,
"(*      modified on Tue Mar  2 17:18:02 PST 1993 by muller            *)", 0,
"", 0,
"(* $Id$ *)", 0,
"", 0,
"INTERFACE Usignal;", 0,
"", 0,
"FROM Ctypes IMPORT int, unsigned_int, unsigned_short_int, unsigned_long_int;", 0,
"IMPORT Uucontext;", 0,
"", 0,
"(*** <signal.h> ***)", 0,
"", 0,
"CONST", 0,
"  (* SIGHUP    =  %u; *) (* hangup *)", SIGHUP,
"  SIGINT    =  %u; (* interrupt *)", SIGINT,
"  SIGQUIT   =  %u; (* quit *)", SIGQUIT,
"  (* SIGILL    =  %u; *) (* illegal instruction (not reset when caught) *)", SIGILL,
"  (* SIGTRAP   =  %u; *) (* trace trap (not reset when caught) *)", SIGTRAP,
"  SIGIOT    =  %u; (* IOT instruction *)", 6,
"  (* Linux 1.1.73 doesn't have SIGEMT - rrw *)", 0,
"  (* SIGEMT    =  %u; *) (* EMT instruction *)", SIGEMT,
"  (* SIGBUS    =  %u; *) (* bus error *)", SIGBUS,
"  (* BUS_HWERR	  = %u; *) (* misc hardware error (e.g. timeout) *)", 1,
"  (* BUS_ALIGN	  = %u; *) (* hardware alignment error *)", 2,
"  (* SIGFPE    =  %u; *) (* floating point exception *)", SIGFPE,
"  (* FPE_INTDIV_TRAP      = %u;    *) (* integer divide by zero *)", 20,
"  (* FPE_INTOVF_TRAP      = %u;    *) (* integer overflow *)", 21,
"  (* FPE_FLTOPERR_TRAP    =  %u;   *) (* [floating operand error] *)", 1,
"  (* FPE_FLTDEN_TRAP      =  %u;   *) (* [floating denormalized operand] *)", 2,
"  (* FPE_FLTDIV_TRAP      =  %u;   *) (* [floating divide by zero] *)", 3,
"  (* FPE_FLTOVF_TRAP      =  %u;   *) (* [floating overflow] *)", 4,
"  (* FPE_FLTUND_TRAP      =  %u;   *) (* [floating underflow] *)", 5,
"  (* FPE_FLTINEX_TRAP     =  %u;   *) (* [floating inexact result] *)", 6,
"  (* FPE_UUOP_TRAP        =  %u;   *) (* [floating undefined opcode] *)", 7,
"  (* FPE_DATACH_TRAP      =  %u;   *) (* [floating data chain exception] *)", 8,
"  (* FPE_FLTSTK_TRAP      = %u;    *) (* [floating stack fault] *)", 16,
"  (* FPE_FPA_ENABLE       = %u;    *) (* [FPA not enabled] *)", 17,
"  (* FPE_FPA_ERROR        = %u;    *) (* [FPA arithmetic exception] *)", 18,
"  (* SIGKILL   =  %u; *) (* kill (cannot be caught or ignored) *)", SIGKILL,
"  (* SIGUSR1   =  %u; *) (* User signal 1 (from SysV) *)", 10,
"  (* SIGSEGV   =  %u; *) (* segmentation violation *)", SIGSEGV,
"  (* SEGV_NOMAP  = %u; *) (* no mapping at the fault address *)", 3,
"  (* SEGV_PROT   = %u; *) (* access exceeded protections *)", 4,
"  (* SEGV_OBJERR = %u; *) (* object returned errno value *)", 5,
"  (* No SIGSYS in Linux 1.1.73 - rrw *)", 0,
"  (* SIGSYS    =  %u; *) (* bad argument to system call *)", SIGSYS,
"  (* SIGUSR2   =  %u; *) (* User signal 2 (from SysV) *)", SIGUSR2,
"  (* SIGPIPE   =  %u; *) (* write on a pipe with no one to read it *)", SIGPIPE,
"  (* SIGALRM   =  %u; *) (* alarm clock *)", SIGALRM,
"  SIGTERM   =  %u; (* software termination signal from kill *)", SIGTERM,
"  (* SIGSTKFLT =  %u; *) ", 16,
"  (* SIGCHLD   =  %u; *) (* to parent on child stop or exit *)", SIGCHLD,
"  (* SIGCONT   =  %u; *) (* continue a stopped process *)", SIGCONT,
"  (* SIGSTOP   =  %u; *) (* sendable stop signal not from tty *)", SIGSTOP,
"  (* SIGTSTP   =  %u; *) (* stop signal from tty *)", SIGTSTP,
"  (* SIGTTIN   =  %u; *) (* to readers pgrp upon background tty read *)", SIGTTIN,
"  (* SIGTTOU   =  %u; *) (* like TTIN for output if (tp->t_local&LTOSTOP) *)", SIGTTOU,
"  (* SIGIO     =  %u; *) (* input/output possible signal *)", SIGIO,
"  (* SIGURG    =  SIGIO; *) (* urgent condition on IO channel *)", 0,
"  (* SIGPOLL   =  SIGIO; *) ", 0,
"  (* SIGXCPU   =  %u; *) (* exceeded CPU time limit *)", SIGXCPU,
"  (* SIGXFSZ   =  %u; *) (* exceeded file size limit *)", SIGXFSZ,
"  SIGVTALRM =  %u; (* virtual time alarm *)", SIGVTALRM,
"  (* SIGPROF   =  %u; *) (* profiling time alarm *)", SIGPROF,
"  (* SIGWINCH  =  %u; *) (* window size changes *)", SIGWINCH,
"  (* SIGLOST is commented out of /usr/include/linux/signal.h in Linux 1.1.73 - rrw *)", 0,
"  (* SIGLOST   =  %u; *) (* Sys-V rec lock: notify user upon server crash *)", SIGLOST,
"  (* Under Linux 1.1.73, signals 30 and 31 are :  - rrw *)", 0,
"  (* SIGPWR     = %u; *)", 30,
"  (* SIGUNUSED  = %u; *)", 31,
"", 0,
"  (* System V definitions *)", 0,
"  (* SIGCLD    = SIGCHLD; *)", 0,
"  SIGABRT   = SIGIOT;", 0,
"", 0,
"CONST", 0,
"  SIGSET_NWORDS = %u;", sizeof(sigset_t) / sizeof(int),
"", 0,
"(* Signal vector \"template\" used in sigaction call. *)", 0,
"TYPE", 0,
"  SignalHandler = PROCEDURE (sig: int;", 0,
"                             scp: struct_sigcontext;", 0,
"                             code: int);", 0,
"", 0,
"  sigset_t = RECORD", 0,
"    val : ARRAY [0 .. SIGSET_NWORDS - 1] OF INTEGER;", 0,
"  END;", 0,
"  sigset_t_star = UNTRACED REF sigset_t;", 0,
"", 0,
"  siginfo_t = RECORD", 0,
"    opaque:  ARRAY [0..%u] OF int;", sizeof(siginfo_t) / sizeof(int),
"  END;", 0,
"", 0,
"  siginfo_t_star = UNTRACED REF siginfo_t;", 0,
"", 0,
"CONST", 0,
"  empty_sigset_t : sigset_t = sigset_t{ARRAY [0..SIGSET_NWORDS - 1] ", 0,
"      OF INTEGER{0, ..}};", 0,
"  empty_sv_mask  : sigset_t = sigset_t{ARRAY [0..SIGSET_NWORDS - 1] ", 0,
"      OF INTEGER{0, ..}};", 0,
"", 0,
"CONST", 0,
"  (* SV_ONSTACK   = 16_%04x ;*)  (* take signal on signal stack *)", 1,
"  (* SV_INTERRUPT = 16_%04x ;*)  (* do not restart system on signal return *)", 2,
"  (* SV_OLDSIG is not provided (explicitly, anyway) by glibc2 *)", 0,
"  (* SV_OLDSIG    = 16_%04x ;*)  (* Emulate old signal() for POSIX *)", 0x1000,
"  (* SV_RESETHAND = 16_%04x ;*)  (* Reset handler to SIG_DFL on receipt *)", 4,
"", 0,
"  (* Defines for sigprocmask() call. POSIX. *)", 0,
"  (* SIG_BLOCK    = %u ;*)    (* Add these signals to block mask *)", SIG_BLOCK,
"  (* SIG_UNBLOCK  = %u ;*)    (* Remove these signals from block mask *)", SIG_UNBLOCK,
"  (* SIG_SETMASK  = %u ;*)    (* Set block mask to this mask *)", SIG_SETMASK,
"", 0,
"TYPE", 0,
"  SignalActionHandler = PROCEDURE (sig: int;", 0,
"                                   sip: siginfo_t_star;", 0,
"                                   uap: Uucontext.ucontext_t_star);", 0,
"  SignalRestoreHandler = PROCEDURE ();", 0,
"", 0,
"  struct_sigaction = RECORD", 0,
"    sa_sigaction: SignalActionHandler;  (* signal handler *)", 0,
"    sa_mask     : sigset_t;             (* signals to block while in handler *)", 0,
"    sa_flags    : int;                  (* signal action flags *)", 0,
"    sa_restorer : SignalRestoreHandler; (* restores interrupted state *)", 0,
"  END;", 0,
"", 0,
"  struct_sigaction_star = UNTRACED REF struct_sigaction;", 0,
"", 0,
" (* valid flags define for sa_flag field of sigaction structure  *)", 0,
"CONST", 0,
"  SA_NOCLDSTOP = %u;           (* Don't generate SIGCLD when children stop *)", SA_NOCLDSTOP,
"  SA_STACK       = 16_%08x;", 0x08000000,
"  SA_RESTART     = 16_%08x;", 0x10000000,
"  SA_INTERRUPT   = 16_%08x;", 0x20000000,
"  SA_NOMASK      = 16_%08x;", 0x40000000,
"  SA_ONESHOT     = 16_%08x;", 0x80000000,
"", 0,
"  SA_ONSTACK     = 16_%04x;   (* run on special signal stack *)", 1,
"  SA_OLDSTYLE    = 16_%04x;   (* old \"unreliable\" UNIX semantics *)", 2,
"  SA_NODUMP      = 16_%04x;   (* termination by this sig does not use a ", 0x10,
"                                 core file *)", 0,
"  SA_PARTDUMP    = 16_%04x;   (* create a partial dump for this signal *)", 0x20,
"  SA_FULLDUMP    = 16_%04x;   (* create a full dump (with data areas) *)", 0x40,
"  SA_SIGSETSTYLE = 16_%04x;   (* new system V sigset type semantics *)", 0x80,
"", 0,
"TYPE", 0,
"  struct_sigstack = RECORD ", 0,
"    ss_sp:      ADDRESS; (* signal stack pointer *)", 0,
"    ss_onstack: int;     (* current status *)", 0,
"  END;", 0,
"", 0,
"(*", 0,
" * Information pushed on stack when a signal is delivered.", 0,
" * This is used by the kernel to restore state following", 0,
" * execution of the signal handler.  It is also made available", 0,
" * to the handler to allow it to properly restore state if", 0,
" * a non-standard exit is performed.", 0,
" *", 0,
" * WARNING: THE sigcontext MUST BE KEPT CONSISTENT WITH /usr/include/setjmp.h", 0,
" * AND THE LIBC ROUTINES setjmp() AND longjmp()", 0,
" *", 0,
" *)", 0,
"", 0,
"TYPE", 0,
"  (* There seems to be no simple corresponding structure under Linux - ", 0,
"      use the structure in Csetjmp.i3 instead *)", 0,
"  struct_sigcontext = RECORD", 0,
"      gs, gsh: unsigned_short_int;", 0,
"      fs, fsh: unsigned_short_int;", 0,
"      es, esh: unsigned_short_int;", 0,
"      ds, dsh: unsigned_short_int;", 0,
"      edi: unsigned_long_int;", 0,
"      esi: unsigned_long_int;", 0,
"      ebp: unsigned_long_int;", 0,
"      esp: unsigned_long_int;", 0,
"      ebx: unsigned_long_int;", 0,
"      edx: unsigned_long_int;", 0,
"      ecx: unsigned_long_int;", 0,
"      eax: unsigned_long_int;", 0,
"      trapno: unsigned_long_int;", 0,
"      err: unsigned_long_int;", 0,
"      eip: unsigned_long_int;", 0,
"      cs, csh: unsigned_short_int;", 0,
"      eflags: unsigned_long_int;", 0,
"      esp_at_signal: unsigned_long_int;", 0,
"      ss, ssh: unsigned_short_int;", 0,
"      i387: unsigned_long_int; (* Actually a struct _fpstate * *)", 0,
"      oldmask: unsigned_long_int;", 0,
"      cr2: unsigned_long_int;", 0,
"    END;", 0,
"  ", 0,
" struct_fpreg = RECORD", 0,
"   significand : ARRAY [0..3] OF unsigned_short_int;", 0,
"   exponent : unsigned_short_int;", 0,
" END;", 0,
"", 0,
" struct_fpstate = RECORD", 0,
"   cw : unsigned_long_int;", 0,
"   sw : unsigned_long_int;", 0,
"   tag : unsigned_long_int;", 0,
"   ipoff : unsigned_long_int;", 0,
"   cssel : unsigned_long_int;", 0,
"   dataoff: unsigned_long_int;", 0,
"   datasel : unsigned_long_int;", 0,
"   st : ARRAY [0..7] OF struct_fpreg;", 0,
"   status : unsigned_long_int;", 0,
" END;  ", 0,
"", 0,
"", 0,
"(* Do not modifiy these variables *)", 0,
"VAR (*CONST*)", 0,
"  BADSIG, SIG_ERR, SIG_DFL, SIG_IGN, SIG_HOLD: SignalActionHandler;", 0,
"", 0,
"", 0,
"(* Convert a signal number to a mask suitable for sigblock(). *)", 0,
"<*INLINE*> PROCEDURE sigmask (n: int): int;", 0,
"", 0,
"", 0,
"(*** kill(2) - send signal to a process ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE kill (pid, sig: int): int;", 0,
"", 0,
"", 0,
"(*** killpg(2) - send signal to a process or process group ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE killpg (pgrp, sig: int): int;", 0,
"", 0,
"", 0,
"(*** sigblock(2) - block signals ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE sigblock (mask: int): int;", 0,
"", 0,
"", 0,
"(*** sigpause(2) - atomically release blocked signals and wait for", 0,
"                   interrupt ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE sigpause (sigmask: int): int;", 0,
"", 0,
"", 0,
"(*** sigpending(2) - examine pending signals ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE sigpending (VAR set: sigset_t): int;", 0,
"", 0,
"", 0,
"(*** sigsetmask(2) - set current signal mask ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE sigsetmask (mask: int): unsigned_int;", 0,
"", 0,
"", 0,
"(*** sigstack(2) - set and/or get signal stack context ***)", 0,
"", 0,
"<*EXTERNAL*> PROCEDURE sigstack (VAR ss, oss: struct_sigstack): int;", 0,
"", 0,
"", 0,
"(*** sigaction(2) - software signal facilities ***)", 0,
"", 0,
"<*EXTERNAL*>", 0,
"PROCEDURE sigaction (sig: int;", 0,
"                     READONLY act: struct_sigaction;", 0,
"                     VAR oact: struct_sigaction): int;", 0,
"", 0,
"(*** sigprocmask(2) - set the blocked signals ***)", 0,
"", 0,
"<*EXTERNAL*>", 0,
"PROCEDURE sigprocmask(how: int; set, oldset: sigset_t_star): int;", 0,
"", 0,
"(*", 0,
"PROCEDURE SigWord(sig : INTEGER) : INTEGER;", 0,
"PROCEDURE SigMask(sig : INTEGER) : Word.T;", 0,
"PROCEDURE SigIsMember(set : sigset_t; sig : INTEGER) : BOOLEAN;", 0,
"PROCEDURE SigAddSet(set : sigset_t; sig : INTEGER) : sigset_t;", 0,
"PROCEDURE SigDelSet(set : sigset_t; sig : INTEGER) : sigset_t;", 0,
"*)", 0,
"", 0,
"(* Change the set of blocked signals to SET,", 0,
"   wait until a signal arrives, and restore the set of blocked signals. *)", 0,
"<*EXTERNAL*> PROCEDURE sigsuspend (READONLY set: sigset_t): int;", 0,
"", 0,
"(* Select any of pending signals from SET or wait for any to arrive.  *)", 0,
"<*EXTERNAL*> PROCEDURE sigwait (READONLY set: sigset_t; VAR sig: int): int;", 0,
"", 0,
"(* Remove SIGNO from SET.  *)", 0,
"<*EXTERNAL*> PROCEDURE sigdelset (VAR set: sigset_t; signo: int): int;", 0,
"", 0,
"(* Set all signals in SET.  *)", 0,
"<*EXTERNAL*> PROCEDURE sigfillset (VAR set: sigset_t): int;", 0,
"", 0,
"(* Bits in `sa_flags'.  *)", 0,
"CONST", 0,
"  SA_SIGINFO   =  %u;	      (* Invoke signal-catching function with", 4,
"				 three arguments instead of one.  *)", 0,
"", 0,
"END Usignal.", 0,
};
    for (i = 0 ; i != sizeof(Data)/sizeof(Data[0]) ; ++i)
    {
        printf(Data[i].Format, Data[i].Value);
        printf("\n");
    }
    return 0;
}
