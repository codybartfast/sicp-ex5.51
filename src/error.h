#ifndef SSERROR_H
#define SSERROR_H 1

#include <stdbool.h>
#include "obj.h"

void eprintf(const char *area, const char *message, ...);

bool iserr(obj obj);
obj error_memory(void);
obj error_argument_type(void);
obj error_internal(void);
obj error_lexor(void);
obj error_parser(void);
obj error_convert(void);
obj error_eval(void);
obj error_write(void);

#endif
