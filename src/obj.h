#ifndef OBJ_H
#define OBJ_H

#include <stdbool.h>
#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_SYMBOL 1
#define TYPE_NUMBER 2
#define TYPE_STRING 3
#define TYPE_EMPTY_LIST 10
#define TYPE_REFERENCE 11
#define TYPE_PRIMITIVE_PROCEDURE 12
#define TYPE_UNSPECIFIED 13
#define TYPE_EOF 14
#define TYPE_ERROR 99

#define SUBTYPE_NOT_SET 0
#define NUMBER_INT64 1

struct obj;

struct simp {
	uint8_t type;
	uint8_t subtype;
	union {
		int64_t int64;
		char *string;
		struct obj *reference;
		struct obj (*primproc)(struct obj);
	} val;
};

// .iserr = iserr
struct pair {
	struct simp car;
	struct simp cdr;
};

typedef struct obj {
	bool ispair;
	union {
		struct simp simp;
		struct pair pair;
	};
} obj;

bool ispair(obj);
bool isnull(obj dat);

obj cons(obj, obj);
obj car(obj);
obj cdr(obj);

struct obj_accessor {
	bool (*issymbol)(obj);
	obj (*ofidentifier)(char *);
	char *(*toidentifier)(obj);

	bool (*isnumber)(obj);
	obj (*ofint64)(int64_t);
	int64_t (*toint64)(obj);

	bool (*isstring)(obj);
	obj (*nl)(void);
	obj (*ofstring)(char *);
	char *(*tostring)(obj);

	bool (*isreference)(obj);
	obj (*reference)(obj);
	obj (*dereference)(obj);

	bool (*isprimproc)(obj);
	obj (*offunction)(obj (*)(obj));
	obj (*(*tofunction)(obj))(obj);

	obj (*empty)(void);

	obj (*unspecified)(void);

	bool (*iseof)(obj);
	obj (*eof)(void);
};

extern const struct obj_accessor Obj;

obj make_err(int err_subtype);

#endif
