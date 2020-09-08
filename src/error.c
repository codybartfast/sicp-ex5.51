#include <stdarg.h>
#include <stdio.h>
#include "sicpstd.h"
#include "output.h"

#include "error.h"

#define AREA "ERROR"

enum errsbtyp {
	ERROR_MEMORY = 1,
	ERROR_ARGUMENT_TYPE, //2
	ERROR_ARGUMENT_VALUE, //3
	ERROR_EVAL, //4
	ERROR_INTERNAL, //5
	ERROR_PARSER, //6
	ERROR_UNBOUND_VARIABLE, //7
	ERROR_USER //8
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

obj error_memory(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_MEMORY, "Out of Memory", area, message,
			      args);
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

obj error_arity(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_ARGUMENT_VALUE, "Arity Error", area,
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

obj error_syntax(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_PARSER, "Syntax Error", area, message,
			      args);
}

obj error_unbound_variable(const char *area, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	return print_make_err(ERROR_UNBOUND_VARIABLE, "Unbound Variable", area,
			      message, args);
}

obj user_error(obj args)
{
	obj dat;
	if (!is_pair(args)) {
		return error_arity(
			AREA, "'error' got no args (expects a message string)");
	}
	dat = car(args);
	if (!is_string(dat)) {
		return error_argument_type(
			AREA,
			"'error' expects a message string as the first argument");
	}
	displaydat(of_string("ERROR: "));
	displaydat(dat);
	for (dat = cdr(args); is_pair(dat); dat = cdr(dat)) {
		displaydat(of_string(" "));
		write(car(dat));
	}
	newline(emptylst);
	return make_err(ERROR_USER);
}
