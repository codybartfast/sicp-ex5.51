#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "sicpstd.h"

#include "obj.h"

obj extend_environment(obj vars, obj vals, obj base_env, obj proc_name);
obj lookup_variable_value(obj exp, obj env);
obj set_variable_value(obj var, obj val, obj env);
obj define_variable(obj var, obj val, obj env);
obj the_global_environment(void);
obj setup_environment(void);
obj tge(void);
void set_global_environment(obj);

#endif
