#ifndef OUTPUT_H
#define OUTPUT_H

#include "obj.h"
#include "outport.h"

obj newline(void);
obj newlinep(struct outport *);
obj write(obj);
obj writep(struct outport *, obj);

#endif
