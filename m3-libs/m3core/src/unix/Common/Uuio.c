/* Copyright (C) 1990, Digital Equipment Corporation.         */
/* All rights reserved.                                       */
/* See the file COPYRIGHT for a full description.             */

#include "m3core.h"

#define M3MODULE Uuio
M3WRAP3_(ssize_t, read, int, void*, WORD_T)
M3WRAP3_(ssize_t, write, int, const void*, WORD_T)
