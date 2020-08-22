#ifndef OUTPUT_H
#define OUTPUT_H
#include "sicpstd.h"

#include "obj.h"
#include "outport.h"

struct outport *default_out(void);

obj displayp(struct outport *, obj);
obj newline(void);
obj newlinep(struct outport *);
obj write(obj);
obj writep(struct outport *, obj);
obj writestr(obj);

#endif
