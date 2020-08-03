#ifndef OBJ_H
#define OBJ_H 1

#include <stdbool.h>
#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_NUMBER 2
#define TYPE_STRING 3
#define TYPE_EOF 4
#define TYPE_ERROR 5

#define NUMBER_INT64 1

union value {
	int64_t int64;
	char *string;
};

typedef struct {
	uint8_t type;
	uint8_t subtype;
	union value val;
} obj;

struct accessors {
	bool (*iserr)(obj *);
	bool (*iseof)(obj *);
	obj *(*eof)(void);
	/* number */
	/* int64 */
	bool (*isnumber)(obj *);
	obj *(*ofint64)(int64_t);
	int64_t (*toint64)(obj *);
	/* string */
	bool (*isstring)(obj *dat);
	obj *(*nl)(void);
	obj *(*ofstring)(char *);
	char *(*tostring)(obj *);
};

extern const struct accessors Obj;

obj *new_obj(void);

#endif
