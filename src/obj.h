#ifndef OBJ_H
#define OBJ_H 1

#include <stdbool.h>
#include <inttypes.h>

#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_NUMBER 2
#define TYPE_STRING 3
#define TYPE_EOF 4
#define TYPE_ERROR 5

union value {
	int64_t int64;
	char *string;
};

typedef struct {
	uint8_t type;
	uint8_t subtype;
	union value val;
} obj;

obj *new_obj(void);

// NUMBER

#define NUMBER_INT64 1

bool isnumber(obj *);
obj *int64(int64_t);

// STRING
bool isstring(obj *);
obj *string(char *);

// EOF
extern obj *eof;
bool iseof(obj *obj);

#endif
