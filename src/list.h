#ifndef LIST_H
#define LIST_H
#include "sicpstd.h"

#include "obj.h"

obj cadr(obj);
obj cddr(obj);
obj caadr(obj);
obj caddr(obj);
obj cdadr(obj);

obj list2(obj, obj);
obj list3(obj, obj, obj);
obj listn(int argc, ...);
obj length(obj);
int length_u(obj);
obj map_u(obj (*)(obj), obj);
obj reverse(obj);

#endif
