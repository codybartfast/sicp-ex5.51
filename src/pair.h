#ifndef PAIR_H
#define PAIR_H

#include "obj.h"

bool ispair(obj);
bool isnull(obj dat);

obj cons(obj, obj);
obj car(obj);
obj cdr(obj);
obj set_car(obj);
obj set_cdr(obj);

#endif
