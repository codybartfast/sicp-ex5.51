#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sserror.h"
#include "convert.h"

#define AREA "CONVERT"
/*
 * interwebs suggest double representation can be over 1,000 chars"
 */
char buff[2048];

obj *cnv_number_string(obj *num)
{
	if (!isnumber(num))
		return error_convert();
	switch (num->subtype) {
	case NUMBER_INT64:
		sprintf(buff, "%ld", num->val.int64);
		char *str = (char *)malloc((strlen(buff) + 1) * sizeof(char));
		if (str == NULL)
			return error_memory();
		strcpy(str, buff);
		return string(str);
	default:
		error(AREA, "BUG: more number subtypes than cases.");
		return error_internal();
	}
}