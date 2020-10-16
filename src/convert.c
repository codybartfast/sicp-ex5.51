#include "convert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "error.h"
#include "outport.h"

#define AREA "CONVERT"

#define BUFFSIZE 2048
char buff[BUFFSIZE];

obj cnv_boolean_string(struct outport *op, obj bl)
{
	if (!is_boolean(bl))
		return error_argument_type(
			AREA, "boolean->string got non-boolean: %s",
			errstr(bl));
	if (is_false(bl)) {
		out_writes(op, "#f");
	} else {
		out_writes(op, "#t");
	}
	return void_o;
}

obj cnv_number_string(struct outport *op, obj num)
{
	if (!is_number(num))
		return error_argument_type(
			AREA, "number->string got non-number: %s", errstr(num));
	switch (subtype(num)) {
	case NUMBER_INTEGER:
		sprintf(buff, "%lld", (long long)to_integer(num));
		break;
	case NUMBER_FLOATING:
		sprintf(buff, "%" LG_PRECISION "Lg",
			(long double)to_floating(num));
		break;
	default:
		return error_internal(AREA,
				      "BUG: no case of number subtype %d.",
				      subtype(num));
	}
	out_writes(op, buff);
	return void_o;
}

obj cnv_bitmap_string(struct outport *op, obj dat)
{
	const struct bitmap *bmp;

	if (!is_bitmap(dat))
		return error_argument_type(
			AREA, "bitmap->string got non-bitmap: %s", errstr(dat));
	bmp = to_bitmap(dat);
	sprintf(buff, "<bitmap %dx%d>", bmp->width, bmp->height);
	out_writes(op, buff);
	return void_o;
}
