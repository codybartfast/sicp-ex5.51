#include <stdlib.h>
#include "obj.h"
#include "sserror.h"

// NUMBER

bool isnumber(obj *dat)
{
	return dat->type == TYPE_NUMBER;
}

obj *int64(int64_t n)
{
	obj *obj = new_obj();
	if (iserr(obj))
		return obj;
	obj->type = TYPE_NUMBER;
	obj->subtype = NUMBER_INT64;
	obj->val.int64 = n;
	return obj;
}

// STRING
static obj nl_struct = { TYPE_STRING, 0, { .string = "\n" } };
obj *nl_object = &nl_struct;

bool isstring(obj *dat)
{
	return dat->type == TYPE_STRING;
}

obj *string(char *str)
{
	obj *dat = new_obj();
	if (iserr(dat))
		return dat;
	dat->type = TYPE_STRING;
	dat->val.string = str;
	return dat;
}

// EOF

static obj eof_struct = { TYPE_EOF, 0, { 0 } };
obj *eof_object = &eof_struct;

bool iseof(obj *dat)
{
	return dat == eof_object;
}

// MEMORY 'MANAGEMENT'

obj *new_obj(void)
{
	obj *dat = (obj *)malloc(sizeof(obj));
	if (dat == NULL)
		return error_memory();
	dat->type = TYPE_NOT_SET;
	return dat;
}
