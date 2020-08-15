#include <stdarg.h>
#include <stdio.h>
#include "sicpstd.h"
#include "output.h"

#include "error.h"

enum errsbtyp {
	ERROR_MEMORY = 1,
	ERROR_ARGUMENT_TYPE,
	ERROR_ARGUMENT_VALUE,
	ERROR_EVAL,
	ERROR_INTERNAL,
	ERROR_PARSER,
	ERROR_UNBOUND_VARIABLE
};

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

static obj mem_error = { false, .simp = { TYPE_ERROR, ERROR_MEMORY, { 0 } } };

obj error_memory(void)
{
	return mem_error;
}

static obj print_make_err(enum errsbtyp est, const char *stmsg,
			  const char *area, const char *message, va_list args)
{
	fprintf(stderr, "ERROR: (");
	fprintf(stderr, "%s", area);
	fprintf(stderr, ") ");
	fprintf(stderr, "%s", stmsg);
	fprintf(stderr, ": ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	return make_err(est);
}

obj error_argument_type(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_ARGUMENT_TYPE, "Type Error", area, message,
			      args);
}

obj error_argument_value(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_ARGUMENT_VALUE, "Value Error", area,
			      message, args);
}

obj error_eval(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_EVAL, "Eval Error", area, message, args);
}

obj error_internal(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_INTERNAL, "Programmer Error", area, message,
			      args);
}

obj error_parser(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_PARSER, "Parsing Error", area, message,
			      args);
}

obj error_unbound_variable(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_UNBOUND_VARIABLE, "Unknown Variable", area,
			      message, args);
}
