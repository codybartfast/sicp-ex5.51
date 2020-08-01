#ifndef HDR_OBJ
#define HDR_OBJ

#include <stdbool.h>
#include <stdint.h>

union value {
	int64_t int64;
	char * dfloat;
};

typedef struct {
	uint8_t type;
	uint8_t subtype;
	union value val;
} obj;

// NUMBER
bool isnumber(obj *);
obj *int64(int64_t);

// EOF
extern obj *eof;
bool iseof(obj *obj);

// ERROR
bool iserr(obj *obj);
obj *error_internal(void);
obj *error_lexor(void);
obj *error_parser(void);
obj *error_eval(void);

#endif
