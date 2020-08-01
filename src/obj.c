#include <stdlib.h>
#include "obj.h"

static const uint8_t TYPE_NOT_SET = 0;
static const uint8_t TYPE_NUMBER = 2;
static const uint8_t TYPE_EOF = 3;
static const uint8_t TYPE_ERROR = 4;

static obj *new_obj(void);

// NUMBER
static const uint8_t NUMBER_INT64 = 1;

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

// EOF
static obj eof_obj = { TYPE_EOF, 0, { 0 } };
obj *eof = &eof_obj;

bool iseof(obj *obj)
{
	return obj == eof;
}

// ERROR
static const uint8_t ERROR_MEMORY = 1;
static const uint8_t ERROR_INTERNAL = 2;
static const uint8_t ERROR_LEXOR = 3;
static const uint8_t ERROR_PARSER = 4;
static const uint8_t ERROR_EVAL = 5;

bool iserr(obj *obj)
{
	return obj->type == TYPE_ERROR;
}

static obj mem_error_obj = { TYPE_ERROR, ERROR_MEMORY, { 0 } };
static obj *mem_error = &mem_error_obj;

static obj *error(int err_subtype)
{
	obj *obj;

	if (iserr(obj = new_obj()))
		return obj;
	obj->type = TYPE_ERROR;
	obj->subtype = err_subtype;
	return obj;
}

obj *error_internal(void)
{
	return error(ERROR_INTERNAL);
}

obj *error_lexor(void)
{
	return error(ERROR_LEXOR);
}

obj *error_parser(void)
{
	return error(ERROR_PARSER);
}

obj *error_eval(void)
{
	return error(ERROR_EVAL);
}

// MEMORY 'MANAGEMENT'

static obj *new_obj(void)
{
	obj *object = (obj *)malloc(sizeof(obj));
	if (object == NULL)
		return mem_error;
	object->type = TYPE_NOT_SET;
	return object;
}
