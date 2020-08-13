#ifndef LIST_H
#define LIST_H
#include "sicpstd.h"

#include "obj.h"

obj cadr(obj);
obj list2(obj, obj);
obj listn(int argc, ...);
obj length(obj);
int length_u(obj);
obj map_u(obj (*)(obj), obj);
obj reverse(obj);

#endif
