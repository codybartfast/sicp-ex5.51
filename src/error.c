#include <stdarg.h>
#include <stdio.h>

#include "error.h"

enum { ERROR_MEMORY,
       ERROR_ARGUMENT,
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

bool iserr(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_ERROR;
}

static obj mem_error = { false, .simp = { TYPE_ERROR, ERROR_MEMORY, { 0 } } };

obj error_memory(void)
{
	return mem_error;
}

obj error_argument()
{
	return make_err(ERROR_ARGUMENT);
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

obj error_convert(void)
{
	return make_err(ERROR_CONVERT);
}

obj error_eval(void)
{
	return make_err(ERROR_EVAL);
}

obj error_write(void)
{
	return make_err(ERROR_WRITE);
}
