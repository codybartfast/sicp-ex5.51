#ifndef PRIMPROC_H
#define PRIMPROC_H
#include "sicpstd.h"

#include "obj.h"

obj chkarity(char *fname, int expct, obj args);

obj add(const obj);
obj sub(const obj);
obj mul(const obj);
obj divd(const obj);

obj rem(const obj);

obj absl(const obj);
obj expn(const obj);
obj logn(const obj);
obj sine(const obj);
obj inc(const obj);
obj dec(const obj);

obj rnd(const obj);
obj runtime(const obj);
obj ticks(const obj);
obj seconds(const obj);

obj lt(const obj);
obj lte(const obj);
obj eqn(const obj);
obj gte(const obj);
obj gt(const obj);

obj and(const obj);
obj or(const obj);
obj not(const obj);

#endif
