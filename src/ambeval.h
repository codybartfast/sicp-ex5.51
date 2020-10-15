#ifndef AMBEVAL_H
#define AMBEVAL_H
#include "sicpstd.h"

#include "obj.h"

obj ambeval(obj exp, obj env, obj succeed, obj fail);
obj ambeval2(obj exp, obj env);

#endif