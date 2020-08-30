#ifndef SICPSTD_H
#define SICPSTD_H

typedef long long Integer;
typedef long double Floating;
#define LG_PRECISION ".15"

#ifdef __MACH__
#define _ANSI_SOURCE
#endif

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#endif
