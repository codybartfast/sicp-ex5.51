#include "convert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define AREA "CONVERT"

#define BUFFSIZE 2048
char buff[BUFFSIZE];

static obj cpystrobj(char *s, char *msg)
{
	char *dup = (char *)malloc((strlen(buff) + 1) * sizeof(char));
	if (dup == NULL)
		return error_memory(AREA, msg);
	strcpy(dup, s);
	return of_string(dup);
}

obj cnv_boolean_string(obj bl)
{
	if (!is_boolean(bl))
		return error_argument_type(
			AREA, "boolean->string got non-boolean: %s",
			errstr(bl));
	if (is_false(bl)) {
		sprintf(buff, "#f");
	} else {
		sprintf(buff, "#t");
	}
	return cpystrobj(buff, "boolean conversion");
}

obj cnv_number_string(obj num)
{
	if (!is_number(num))
		return error_argument_type(
			AREA, "number->string got non-number: %s", errstr(num));
	switch (subtype(num)) {
	case NUMBER_INTEGER:
		sprintf(buff, "%lld", (long long)to_integer(num));
		break;
	case NUMBER_FLOATING:
		sprintf(buff, "%.15Lg", (long double)to_floating(num));
		break;
	default:
		return error_internal(AREA,
				      "BUG: no case of number subtype %d.",
				      subtype(num));
	}
	return cpystrobj(buff, "number conversion");
}
