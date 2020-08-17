#ifndef EVAL_H
#define EVAL_H
#include "sicpstd.h"

#include "environment.h"
#include "obj.h"

obj eval(obj expr, obj env);
bool is_compound_procedure(obj exp);
obj procedure_parameters(obj p);
obj procedure_body(obj p);

#endif
