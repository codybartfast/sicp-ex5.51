#include "convert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define AREA "CONVERT"

#define BUFFSIZE 2048
char buff[BUFFSIZE];

obj cnv_number_string(obj num)
{
	char *str;
	size_t slen;

	if (!is_number(num))
		return error_convert();
	switch (subtype(num)) {
	case NUMBER_INTEGER:
		sprintf(buff, "%lld", (long long)to_integer(num));
		break;
	case NUMBER_FLOATING:
		sprintf(buff, FLOATING_FORMAT, to_floating(num));
		break;
	default:
		eprintf(AREA, "BUG: no case of number subtype %d.",
			subtype(num));
		return error_internal();
	}
	slen = strlen(buff);
	str = (char *)malloc((slen + 1) * sizeof(char));
	if (str == NULL)
		return error_memory();
	strcpy(str, buff);
	return Obj.of_string(str);
}
