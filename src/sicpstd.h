#ifndef SICPSTD_H
#define SICPSTD_H

#ifdef __MACH__
#define _ANSI_SOURCE
#endif

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include <stdint.h>

typedef int64_t Integer;
typedef double Floating;
#define LG_PRECISION ".15"


#endif
