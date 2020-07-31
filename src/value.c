#include "value.h"


#include <stdio.h>

enum type { NUMBER };

enum number_type { INT32 };

void int32_value(int32_t n, value *v)
{
	v->type = NUMBER;
	v->subtype = INT32;
	v->data = n;
}

int32_t value_int32(value *v){
	return v->data;
}
