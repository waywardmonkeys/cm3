/* Copyright (C) 1989, Digital Equipment Corporation           */
/* All rights reserved.                                        */
/* See the file COPYRIGHT for a full description.              */

#include "m3core.h"

#define M3MODULE Cstring

M3WRAP3(const void*, memchr, const void*, int, WORD_T)
M3WRAP3(void*, memcpy, void*, const void*, WORD_T)
M3WRAP3(void*, memset, void*, int, WORD_T)
M3WRAP3(int, memcmp, const void*, const void*, WORD_T)
M3WRAP3(char*, strncpy, char*, const char*, WORD_T)
M3WRAP3(char*, strncat, char*, const char*, WORD_T)
M3WRAP2(const char*, strchr, const char*, int)
M3WRAP2(const char*, strrchr, const char*, int)
M3WRAP2(const char*, strpbrk, const char*, const char*)
M3WRAP2(char*, strtok, char*, const char*)
M3WRAP2(int, strcmp, const char*, const char*)
M3WRAP3(int, strncmp, const char*, const char*, WORD_T)
M3WRAP1(WORD_T, strlen, const char*)
M3WRAP2(WORD_T, strspn, const char*, const char*)
M3WRAP2(WORD_T, strcspn, const char*, const char*)
M3WRAP3(void*, memmove, void*, const void*, WORD_T)
M3WRAP2(int, strcoll, const char*, const char*)
M3WRAP3(WORD_T, strxfrm, char*, const char*, WORD_T)
M3WRAP2(const char*, strstr, const char*, const char*)
M3WRAP1(char*, strerror, int)
