#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "convert.h"
#include "windows.h"

#define AREA "CONVERT"

#define BUFFSIZE 2048
char buff[BUFFSIZE];

obj cnv_number_string(obj num)
{
	char *str;
	size_t slen;

	if (!isnumber(num))
		return error_convert();
	switch (subtype(num)) {
	case NUMBER_INT64:
		sprintf_s(buff, BUFFSIZE, "%lld", (long long)Obj.toint64(num));
		slen = strlen(buff);
		str = (char *)malloc((slen + 1) * sizeof(char));
		if (str == NULL)
			return error_memory();
		strcpy_s(str, slen + 1, buff);
		return Obj.ofstring(str);
	default:
		eprintf(AREA, "BUG: no case of number subtype %d.",
			subtype(num));
		return error_internal();
	}
}
