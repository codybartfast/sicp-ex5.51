#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "obj.h"

obj lookup_variable_value(obj exp, obj env);
obj the_global_environment(void);

#endif
