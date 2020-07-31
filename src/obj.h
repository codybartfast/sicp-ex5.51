#include <stdbool.h>
#include <stdint.h>

union value {
	int64_t int64;
};

typedef struct {
	uint8_t type;
	uint8_t subtype;
	union value value;
} obj;

obj *new_obj(void);

// NUMBER
obj *int64_obj(int64_t, obj *);
int64_t obj_int64(obj *);

// STATIC
extern obj *eof;
bool iseof(obj *obj);

// ERROR
bool iserr(obj *obj);
obj *error_internal(void);
obj *error_lexor(void);
obj *error_parser(void);
