#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "sicpstd.h"

#include "obj.h"

obj lookup_variable_value(obj exp, obj env);
obj define_variable(obj var, obj val, obj env);
obj the_global_environment(void);

#endif
