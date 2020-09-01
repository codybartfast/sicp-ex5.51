#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "sicpstd.h"

#include "obj.h"

bool eq_symbol(obj a, obj b);
obj extend_environment(obj vars, obj vals, obj base_env);
obj lookup_variable_value(obj exp, obj env);
obj define_variable(obj var, obj val, obj env);
obj the_global_environment(void);
void set_global_environment(obj);

#endif
