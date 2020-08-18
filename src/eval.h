#ifndef EVAL_H
#define EVAL_H
#include "sicpstd.h"

#include "environment.h"
#include "obj.h"

obj eval(obj, obj);
bool is_compound_procedure(obj);
obj procedure_parameters(obj);
obj procedure_body(obj);

#endif
