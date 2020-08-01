#ifndef SSERROR_H
#define SSERROR_H 1

#include <stdbool.h>
#include "obj.h"

void error(const char *area, const char *message, ...);

// ERROR

#define ERROR_MEMORY 1
#define ERROR_INTERNAL 2
#define ERROR_LEXOR 3
#define ERROR_PARSER 4
#define ERROR_CONVERT 5
#define ERROR_EVAL 6

bool iserr(obj *obj);
obj *error_memory(void);
obj *error_internal(void);
obj *error_lexor(void);
obj *error_parser(void);
obj *error_convert(void);
obj *error_eval(void);

#endif
