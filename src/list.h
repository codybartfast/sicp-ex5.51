#ifndef LIST_H
#define LIST_H
#include "sicpstd.h"

#include "obj.h"

obj list1(obj);
obj list2(obj, obj);
obj list3(obj, obj, obj);
obj list4(obj, obj, obj, obj);
obj listn(int argc, ...);
obj length(obj);
int length_u(obj);
obj map_u(obj (*)(obj), obj);
obj reverse(obj);
obj append(obj, obj);

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

#endif
