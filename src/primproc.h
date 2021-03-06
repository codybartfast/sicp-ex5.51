#ifndef PRIMPROC_H
#define PRIMPROC_H
#include "sicpstd.h"

#include "obj.h"

obj chkarity(char *fname, int expct, obj args);
// obj chkarityrng(char *fname, obj args, int min, int max, int *act);

obj add(const obj);
obj sub(const obj);
obj mul(const obj);
obj divd(const obj);
obj minimum(const obj);
obj maximum(const obj);
obj rem(const obj);
// obj expt(const obj);

obj absl(const obj);
obj expn(const obj);
obj logn(const obj);
obj inc(const obj);
obj dec(const obj);
obj sine(const obj);
obj cosine(const obj);
obj arctan(const obj);
obj flr(const obj);
obj sqroot(const obj);

Integer plat_rand(void);
obj rnd(const obj);
obj runtime(const obj);
obj ticks(const obj);
obj seconds(const obj);

obj lt(const obj);
obj lte(const obj);
obj eqn(const obj);
obj gte(const obj);
obj gt(const obj);

obj not(const obj);

obj cons_p(const obj);
obj car_p(const obj);
obj set_car_p(const obj);
obj cdr_p(const obj);
obj set_cdr_p(const obj);

bool is_eq(const obj, const obj);
obj is_eq_p(const obj);

bool is_equal(const obj, const obj);
obj is_equal_p(const obj);

obj is_integer_p(obj);
obj is_number_p(obj);
obj is_string_p(obj);
obj is_symbol_p(obj);

obj string_append_p(obj);
obj number_to_string_p(obj);
obj string_to_symbol_p(obj);
obj symbol_to_string_p(obj);

obj string_lt_p(obj);

obj pcnt_ex(obj);

#endif
