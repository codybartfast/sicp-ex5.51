#include <stdlib.h>
#include "obj.h"
#include "sserror.h"

#define VALUE val /* allow easy renaming of value member to check api leak */

static bool iseof(obj *);
static obj *eof(void);
static bool isnumber(obj *);
static obj *ofint64(int64_t);
static int64_t toint64(obj *);
static bool isstring(obj *);
static obj *nl(void);
static char *tostring(obj *);
static obj *ofstring(char *);

const struct accessors Obj = { .iserr = iserr,
			       .iseof = iseof,
			       .eof = eof,
			       .isnumber = isnumber,
			       .ofint64 = ofint64,
			       .toint64 = toint64,
			       .isstring = isstring,
			       .nl = nl,
			       .ofstring = ofstring,
			       .tostring = tostring };

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
	obj *obj = new_obj();
	if (iserr(obj))
		return obj;
	obj->type = TYPE_NUMBER;
	obj->subtype = NUMBER_INT64;
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

static obj nl_struct = { TYPE_STRING, 0, { .string = "\n" } };

static obj *nl(void)
{
	return &nl_struct;
}

static obj *ofstring(char *str)
{
	obj *dat = new_obj();
	if (iserr(dat))
		return dat;
	dat->type = TYPE_STRING;
	dat->VALUE.string = str;
	return dat;
}

static char *tostring(obj *dat)
{
	return dat->VALUE.string;
}

// MEMORY 'MANAGEMENT'

obj *new_obj(void)
{
	obj *dat = (obj *)malloc(sizeof(obj));
	if (dat == NULL)
		return error_memory();
	*dat = (obj){ 0 };
	return dat;
}
