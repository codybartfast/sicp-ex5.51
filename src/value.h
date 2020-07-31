#include <stdint.h>

typedef struct value {
	unsigned int data : 32;
	unsigned int unused : 20;
	unsigned int storage : 4;
	unsigned int subtype : 4;
	unsigned int type : 4;
} value;

void int32_value(int32_t, value *);
int32_t value_int32(value *);
