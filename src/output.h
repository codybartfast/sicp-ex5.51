#ifndef WRITE_H
#define WRITE_H

#include "obj.h"

void newline(void);
void newlinep(struct out_port *);
void write(obj *);
void writep(struct out_port *, obj *);

#endif
