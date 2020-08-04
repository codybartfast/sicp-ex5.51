#include "windows.h"

#ifndef _WIN32

#include <string.h>
#include <stdarg.h>

int fopen_s(FILE **fpp, const char *filename, const char *mode)
{
	*fpp = fopen(filename, mode);
	return (*fpp == NULL) ? 1 : 0;
}

int sprintf_s(char *buffer, size_t buffsize, const char *format, ...)
{
	long writecnt;

	va_list myargs;
	va_start(myargs, format);
	writecnt = vsprintf(buffer, format, myargs);
	buffer[buffsize - 1] = '\0';
	return writecnt >= 0 ? writecnt : -1;
}

int strcpy_s(char *dest, long destsize, const char *src)
{
	int rc;

	rc = (strcpy(dest, src) == NULL);
	dest[destsize - 1] = '\0';
	return rc;
}

#endif
