/* Copyright (C) 1990, Digital Equipment Corporation           */
/* All rights reserved.                                        */
/* See the file COPYRIGHT for a full description.              */

#include "m3unix.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32

int Upthread__detach(m3_pthread_t thread)
{
    return pthread_detach(PTHREAD_FROM_M3(thread));
}

m3_pthread_t Upthread__self(void)
{
    pthread_t a = pthread_self();
    return PTHREAD_TO_M3(a);
}

int Upthread__equal(m3_pthread_t t1, m3_pthread_t t2)
{
    return pthread_equal(PTHREAD_FROM_M3(t1), PTHREAD_FROM_M3(t2));
}

int Upthread__kill(m3_pthread_t thread, int sig)
{
    return pthread_kill(PTHREAD_FROM_M3(thread), sig);
}

#endif

#ifdef __cplusplus
}
#endif
