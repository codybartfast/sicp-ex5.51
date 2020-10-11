#ifndef EVAL_H
#define EVAL_H
#include "sicpstd.h"

#include "obj.h"

extern obj (*eval)(obj, obj) ;

void use_ambeval(void);
void use_aneval(void);

#endif