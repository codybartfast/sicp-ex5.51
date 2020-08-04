#include <stdarg.h>
#include <stdio.h>

#include "error.h"

enum { ERROR_MEMORY,
       ERROR_INTERNAL,
       ERROR_LEXOR,
       ERROR_PARSER,
       ERROR_CONVERT,
       ERROR_EVAL,
       ERROR_WRITE };

void eprintf(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	fprintf(stderr, "ERROR: ");
	fprintf(stderr, "%s", area);
	fprintf(stderr, ": ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	va_end(args);
}

bool iserr(obj *obj)
{
	return obj->type == TYPE_ERROR;
}

static obj mem_error = { TYPE_ERROR, ERROR_MEMORY, { 0 } };

obj *error_memory(void)
{
	return &mem_error;
}

static obj *make_error(int err_subtype)
{
	obj *obj;

	if (iserr(obj = new_obj(TYPE_ERROR, err_subtype)))
		return obj;
	return obj;
}

obj *error_internal(void)
{
	return make_error(ERROR_INTERNAL);
}

obj *error_lexor(void)
{
	return make_error(ERROR_LEXOR);
}

obj *error_parser(void)
{
	return make_error(ERROR_PARSER);
}

obj *error_convert(void)
{
	return make_error(ERROR_CONVERT);
}

obj *error_eval(void)
{
	return make_error(ERROR_EVAL);
}

obj *error_write(void)
{
	return make_error(ERROR_WRITE);
}
