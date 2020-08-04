#include <stdlib.h>
#include <string.h>
#include "obj.h"
#include "error.h"

#define AREA "OBJ"
#define VALUE val /* allow easy renaming of value member to check api leak */

// EOF

static obj eof_struct = { TYPE_EOF, 0, { 0 } };

static bool iseof(obj *dat)
{
	return dat->type == TYPE_EOF;
}

static obj *eof(void)
{
	return &eof_struct;
}

// NUMBER

static bool isnumber(obj *dat)
{
	return dat->type == TYPE_NUMBER;
}

static obj *ofint64(int64_t n)
{
	obj *obj = new_obj(TYPE_NUMBER, NUMBER_INT64);
	if (iserr(obj))
		return obj;
	obj->VALUE.int64 = n;
	return obj;
}

static int64_t toint64(obj *dat)
{
	return dat->VALUE.int64;
}

// STRING

static bool isstring(obj *dat)
{
	return dat->type == TYPE_STRING;
}

static obj newline_struct = { TYPE_STRING, 0, { .string = "\n" } };

static obj *newline(void)
{
	return &newline_struct;
}

static obj *ofstring(char *str)
{
	obj *dat = new_obj(TYPE_STRING, SUBTYPE_NOT_SET);
	if (iserr(dat))
		return dat;
	dat->VALUE.string = str;
	return dat;
}

static char *tostring(obj *dat)
{
	return dat->VALUE.string;
}

// MEMORY 'MANAGEMENT'

obj *new_obj(int type, int subtype)
{
	obj *dat = (obj *)malloc(sizeof(obj));
	if (dat == NULL){
		eprintf(AREA, "No memory for object");
		return error_memory();
	}
	obj tmp = (obj){ .type = type, .subtype = subtype };
	memcpy(dat, &tmp, sizeof *dat);
	return dat;
}

// Accessor

const struct accessors Obj = { .iserr = iserr,
			       .iseof = iseof,
			       .eof = eof,
			       .isnumber = isnumber,
			       .ofint64 = ofint64,
			       .toint64 = toint64,
			       .isstring = isstring,
			       .newline = newline,
			       .ofstring = ofstring,
			       .tostring = tostring };
