/* Copyright (C) 1992, Digital Equipment Corporation                         */
/* All rights reserved.                                                      */
/* See the file COPYRIGHT for a full description.                            */

#include "m3unix.h"
#define M3MODULE Umman

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32

M3WRAP3(int, mprotect, ADDRESS, size_t, int)
M3WRAP6(ADDRESS, mmap, ADDRESS, size_t, int, int, int, m3_off_t)
M3WRAP2(int, munmap, ADDRESS, size_t)

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
