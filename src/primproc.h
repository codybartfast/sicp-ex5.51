#ifndef PRIMPROC_H
#define PRIMPROC_H
#include "sicpstd.h"

#include "obj.h"

obj chkarity(char *fname, int expct, obj args);

obj add_pp(const obj);
obj sub_pp(const obj);
obj mul_pp(const obj);
obj div_pp(const obj);

obj rem_pp(const obj);

obj abs_pp(const obj);
obj exp_pp(const obj);
obj log_pp(const obj);
obj inc_pp(const obj);
obj dec_pp(const obj);

obj random_pp(const obj);
obj runtime_pp(const obj);
obj ticks_pp(const obj);
obj seconds_pp(const obj);

obj lt_pp(const obj);
obj lte_pp(const obj);
obj eqn_pp(const obj);
obj gte_pp(const obj);
obj gt_pp(const obj);

obj and_pp(const obj);
obj or_pp(const obj);
obj not_pp(const obj);

#endif
