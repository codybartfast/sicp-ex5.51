#ifndef STORAGE_H
#define STORAGE_H
#include "sicpstd.h"

#include <stdbool.h>
#include "obj.h"

struct pair *newpair(bool gc_safe);
extern bool disable_gc;

#endif
