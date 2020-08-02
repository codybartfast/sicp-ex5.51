#include <stdlib.h>
#include "obj.h"
#include "sserror.h"

// NUMBER

bool isnumber(obj *obj)
{
	return obj->type == TYPE_NUMBER;
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

bool isstring(obj *obj)
{
	return obj->type == TYPE_STRING;
}

obj *string(char *str)
{
	obj *obj = new_obj();
	if (iserr(obj))
		return obj;
	obj->type = TYPE_STRING;
	obj->val.string = str;
	return obj;
}

// EOF

static obj eof_struct = { TYPE_EOF, 0, { 0 } };
obj *eof_object = &eof_struct;

bool iseof(obj *obj)
{
	return obj == eof_object;
}

// MEMORY 'MANAGEMENT'

obj *new_obj(void)
{
	obj *object = (obj *)malloc(sizeof(obj));
	if (object == NULL)
		return error_memory();
	object->type = TYPE_NOT_SET;
	return object;
}
