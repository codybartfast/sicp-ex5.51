#ifndef LIST_H
#define LIST_H

#include "obj.h"

obj list(obj, obj);
obj length(obj);
int length_u(obj);
obj map_u(obj (*)(obj), obj);
obj reverse(obj);

#endif
