#ifndef LOAD_H
#define LOAD_H
#include "sicpstd.h"

#include "inport.h"
#include "obj.h"
#include "outport.h"

obj load_u(struct inport *in, struct outport *out, obj *unev, bool verbose);
obj loadq(obj);
obj loadv(obj);

#endif
