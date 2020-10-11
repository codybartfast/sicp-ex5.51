#ifndef ANEVAL_H
#define ANEVAL_H
#include "sicpstd.h"

#include "obj.h"

void add_primprocs(obj env);
obj aneval(obj exp, obj env);

#endif