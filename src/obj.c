#include <stdlib.h>
#include "obj.h"
#include "sserror.h"

// NUMBER

bool isnumber(obj *obj){
	return obj->type = TYPE_NUMBER;
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

bool isstring(obj *obj){
	return obj->type = TYPE_STRING;
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

static obj eof_obj = { TYPE_EOF, 0, { 0 } };
obj *eof = &eof_obj;

bool iseof(obj *obj)
{
	return obj == eof;
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
