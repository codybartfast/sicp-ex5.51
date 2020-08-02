#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "sserror.h"

#define ERROR_MEMORY 1
#define ERROR_INTERNAL 2
#define ERROR_LEXOR 3
#define ERROR_PARSER 4
#define ERROR_CONVERT 5
#define ERROR_EVAL 6
#define ERROR_WRITE 7

void error(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	fprintf(stderr, "error: ");
	fprintf(stderr, "%s", area);
	fprintf(stderr, ": ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	va_end(args);
}

// ERROR

bool iserr(obj *obj)
{
	return obj->type == TYPE_ERROR;
}

static obj mem_error_obj = { TYPE_ERROR, ERROR_MEMORY, { 0 } };
static obj *mem_error = &mem_error_obj;

obj *error_memory(void){
	return mem_error;
}

static obj *make_error(int err_subtype)
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
