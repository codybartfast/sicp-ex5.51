#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sserror.h"
#include "convert.h"
#include "windows.h"

#define AREA "CONVERT"
/*
 * interwebs suggest double representation can be over 1,000 chars"
 */
#define BUFFSIZE 2048
char buff[BUFFSIZE];

obj *cnv_number_string(obj *num)
{
	size_t slen;
	if (!Obj.isnumber(num))
		return error_convert();
	switch (num->subtype) {
	case NUMBER_INT64:
		sprintf_s(buff, BUFFSIZE, "%lld", (long long)Obj.toint64(num));
		//sprintf(buff, "%ld", Obj.toint64(num));
		slen = strlen(buff);
		char *str = (char *)malloc((slen + 1) * sizeof(char));
		if (str == NULL)
			return error_memory();
		strcpy_s(str, slen + 1, buff);
		//strcpy(str, buff);
		return Obj.ofstring(str);
	default:
		error(AREA, "BUG: more number subtypes than cases.");
		return error_internal();
	}
}
