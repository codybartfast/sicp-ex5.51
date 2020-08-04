#ifndef OBJ_H
#define OBJ_H 1

#include <stdbool.h>
#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_NUMBER 1
#define TYPE_STRING 2
#define TYPE_EOF 3
#define TYPE_ERROR 4

#define SUBTYPE_NOT_SET 0
#define NUMBER_INT64 1

union value {
	int64_t int64;
	char *string;
};

typedef struct {
	const uint8_t type;
	const uint8_t subtype;
	union value val;
} obj;

struct accessors {
	bool (*iserr)(obj *);

	bool (*iseof)(obj *);
	obj *(*eof)(void);

	bool (*isnumber)(obj *);
	obj *(*ofint64)(int64_t);
	int64_t (*toint64)(obj *);

	bool (*isstring)(obj *dat);
	obj *(*newline)(void);
	obj *(*ofstring)(char *);
	char *(*tostring)(obj *);
};

extern const struct accessors Obj;

obj *make_err(int err_subtype);

#endif
