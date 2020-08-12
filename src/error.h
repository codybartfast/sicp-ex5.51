#ifndef ERROR_H
#define ERROR_H
#include "sicpstd.h"

#include <stdbool.h>
#include "obj.h"

void eprintf(const char *area, const char *message, ...);

bool is_err(obj obj);
obj error_memory(void);
obj error_argument_type(void);
obj error_convert(void);
obj error_eval(void);
obj error_internal(void);
obj error_lexor(void);
obj error_parser(void);
obj error_unbound_variable(void);
obj error_write(void);

#endif
