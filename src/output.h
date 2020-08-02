#ifndef OUTPUT_H
#define OUTPUT_H

#include "obj.h"
#include "outport.h"

void newline(void);
void newlinep(struct out_port *);
void write(obj *);
void writep(struct out_port *, obj *);

#endif
