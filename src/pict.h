#ifndef PICT_H
#define PICT_H
#include "sicpstd.h"

#include "obj.h"

obj paint(obj);
obj draw_line(obj args);
obj write_canvas(obj);
obj write_canvas_if_painted(obj);

#endif