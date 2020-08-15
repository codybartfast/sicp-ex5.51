#include <stdarg.h>
#include <stdio.h>
#include "sicpstd.h"
#include "output.h"

#include "error.h"

enum { ERROR_MEMORY = 1,
       ERROR_ARGUMENT_TYPE,
       ERROR_ARGUMENT_VALUE,
       ERROR_CONVERT,
       ERROR_EVAL,
       ERROR_INTERNAL,
       ERROR_LEXOR,
       ERROR_PARSER,
       ERROR_UNBOUND_VARIABLE,
       ERROR_WRITE };

const char *errstr(obj dat)
{
	return to_string(writestr(dat));
}

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

bool is_err(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_ERROR;
}

static obj mem_error = { false, .simp = { TYPE_ERROR, ERROR_MEMORY, { 0 } } };

obj error_memory(void)
{
	return mem_error;
}

obj error_argument_type(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	fprintf(stderr, "ERROR: ");
	fprintf(stderr, "%s", area);
	fprintf(stderr, ": ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	return make_err(ERROR_ARGUMENT_TYPE);
}

obj error_argument_value(void)
{
	return make_err(ERROR_ARGUMENT_VALUE);
}

obj error_convert(void)
{
	return make_err(ERROR_CONVERT);
}

obj error_eval(void)
{
	return make_err(ERROR_EVAL);
}

obj error_internal(void)
{
	return make_err(ERROR_INTERNAL);
}

obj error_lexor(void)
{
	return make_err(ERROR_LEXOR);
}

obj error_parser(void)
{
	return make_err(ERROR_PARSER);
}

obj error_unbound_variable(void)
{
	return make_err(ERROR_UNBOUND_VARIABLE);
}

obj error_write(void)
{
	return make_err(ERROR_WRITE);
}
