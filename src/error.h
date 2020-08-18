#ifndef ERROR_H
#define ERROR_H
#include "sicpstd.h"

#include <stdbool.h>
#include "obj.h"

const char *errstr(obj dat);
void eprintf(const char *area, const char *message, ...);

obj error_memory(const char *area, const char *message, ...);
obj error_argument_type(const char *area, const char *message, ...);
obj error_argument_value(const char *area, const char *message, ...);
obj error_arity(const char *area, const char *message, ...);
obj error_eval(const char *area, const char *message, ...);
obj error_internal(const char *area, const char *message, ...);
obj error_parser(const char *area, const char *message, ...);
obj error_syntax(const char *area, const char *message, ...);
obj error_unbound_variable(const char *area, const char *message, ...);

#endif
