#include <stdlib.h>
#include "obj.h"

static const uint8_t TYPE_NOT_SET = 0;
static const uint8_t TYPE_NUMBER = 2;
static const uint8_t TYPE_STATIC = 3;
static const uint8_t TYPE_ERROR = 4;

// NUMBER
static uint8_t NUMBER_INT64 = 1;
obj *error_mem(void);

obj *new_obj(void)
{
	obj *object = (obj *)malloc(sizeof(obj));
	if (object == NULL)
		return error_mem();
	object->type = TYPE_NOT_SET;
	return object;
}

obj *int64_obj(int64_t n, obj *obj)
{
	obj->type = TYPE_NUMBER;
	obj->subtype = NUMBER_INT64;
	obj->value.int64 = n;
	return obj;
}

int64_t obj_int64(obj *v)
{
	return v->value.int64;
}
// STATIC
static obj eof_obj = { TYPE_STATIC, 0, { 0 } };
obj *eof = &eof_obj;
bool iseof(obj *obj)
{
	return obj == &eof_obj;
}

// ERROR
static uint8_t ERROR_MEMORY = 1;
static uint8_t ERROR_INTERNAL = 2;
static uint8_t ERROR_LEXOR = 3;
static uint8_t ERROR_PARSER = 4;
static obj mem_error_obj;

bool iserr(obj *obj)
{
	return obj->type == TYPE_ERROR;
}

obj *error_mem(void)
{
	mem_error_obj.type = TYPE_ERROR;
	mem_error_obj.subtype = ERROR_MEMORY;
	mem_error_obj.value.int64 = 0;
	return &mem_error_obj;
}

static obj *error(int err_subtype);
obj *error(int err_subtype)
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
