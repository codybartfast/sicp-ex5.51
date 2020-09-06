#ifndef LIST_H
#define LIST_H
#include "sicpstd.h"

#include "obj.h"

obj list1(obj);
obj list2(obj, obj);
obj list3(obj, obj, obj);
obj list4(obj, obj, obj, obj);
obj listn(int argc, ...);
obj list(obj);
obj is_null_p(obj);
obj length(obj);
int length_i(obj, int, bool);
int length_u(obj);
obj length_p(obj);
obj map_u(obj (*)(obj), obj);
obj reverse(obj);
obj reverse_p(obj);
obj append(obj, obj);
obj append_p(obj);

// Internal

obj caar(obj lst);
obj cadr(obj lst);
obj cdar(obj lst);
obj cddr(obj lst);

obj caaar(obj lst);
obj caadr(obj lst);
obj cadar(obj lst);
obj caddr(obj lst);
obj cdaar(obj lst);
obj cdadr(obj lst);
obj cddar(obj lst);
obj cdddr(obj lst);

obj caaaar(obj lst);
obj caaadr(obj lst);
obj caadar(obj lst);
obj caaddr(obj lst);
obj cadaar(obj lst);
obj cadadr(obj lst);
obj caddar(obj lst);
obj cadddr(obj lst);
obj cdaaar(obj lst);
obj cdaadr(obj lst);
obj cdadar(obj lst);
obj cdaddr(obj lst);
obj cddaar(obj lst);
obj cddadr(obj lst);
obj cdddar(obj lst);
obj cddddr(obj lst);

// Primitive

obj caar_p(obj lst);
obj cadr_p(obj lst);
obj cdar_p(obj lst);
obj cddr_p(obj lst);

obj caaar_p(obj lst);
obj caadr_p(obj lst);
obj cadar_p(obj lst);
obj caddr_p(obj lst);
obj cdaar_p(obj lst);
obj cdadr_p(obj lst);
obj cddar_p(obj lst);
obj cdddr_p(obj lst);

obj caaaar_p(obj lst);
obj caaadr_p(obj lst);
obj caadar_p(obj lst);
obj caaddr_p(obj lst);
obj cadaar_p(obj lst);
obj cadadr_p(obj lst);
obj caddar_p(obj lst);
obj cadddr_p(obj lst);
obj cdaaar_p(obj lst);
obj cdaadr_p(obj lst);
obj cdadar_p(obj lst);
obj cdaddr_p(obj lst);
obj cddaar_p(obj lst);
obj cddadr_p(obj lst);
obj cdddar_p(obj lst);
obj cddddr_p(obj lst);

#endif
