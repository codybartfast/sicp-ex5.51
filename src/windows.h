#ifndef WINDOWS_H
#define WINDOWS_H
#include "sicpstd.h"

#ifndef _WIN32

#include <stdio.h>

int fopen_s(FILE **fpp, const char *filename, const char *mode);

// #define sprintf_s(B, S, F, ...) sprintf(B, F, __VA_ARGS__)
int sprintf_s(char *buffer, size_t buffsize, const char *format, ...);

int strcpy_s(char *dest, long destsize, const char *src);

#endif

#endif
