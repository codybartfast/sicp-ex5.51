#ifndef CONVERT_H
#define CONVERT_H
#include "sicpstd.h"

#include "obj.h"
#include "outport.h"

obj cnv_boolean_string(struct outport *, obj);
obj cnv_number_string(struct outport *, obj);
obj cnv_bitmap_string(struct outport *, obj);

#endif
