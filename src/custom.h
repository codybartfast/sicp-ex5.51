#ifndef CUSTOM_H
#define CUSTOM_H
#include "sicpstd.h"

#include "inport.h"
#include "obj.h"
#include "output.h"

obj do_head(obj exp, obj env, struct inport *);
obj display_defined(obj);

#endif
