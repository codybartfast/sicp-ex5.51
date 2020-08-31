#ifndef EVAL_H
#define EVAL_H
#include "sicpstd.h"

//#include "environment.h"
#include "obj.h"

obj eval(obj exp, obj env);
obj root(void);

#endif
