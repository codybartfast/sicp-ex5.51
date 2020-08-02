#ifndef OUTPUT_H
#define OUTPUT_H

#include "obj.h"
#include "outport.h"

void newline(void);
void newlinep(struct outport *);
void write(obj *);
void writep(struct outport *, obj *);

#endif
