#ifndef OBJ_H
#define OBJ_H

#include <stdbool.h>
#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_NUMBER 1
#define TYPE_STRING 2
#define TYPE_EMPTY_LIST 4
#define TYPE_REFERENCE 5
#define TYPE_UNSPECIFIED 6
#define TYPE_EOF 7
#define TYPE_ERROR 8

#define SUBTYPE_NOT_SET 0
#define NUMBER_INT64 1

struct obj;

struct simp {
	uint8_t type;
	uint8_t subtype;
	union {
		int64_t int64;
		char *string;
		struct obj *pointer;
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
// obj set_car(obj *, obj);
// obj set_cdr(obj *, obj);

struct obj_accessor {
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

	obj (*empty)(void);

	obj (*unspecified)(void);

	bool (*iseof)(obj);
	obj (*eof)(void);

	// bool (*iserr)(obj);
};

extern const struct obj_accessor Obj;

obj make_err(int err_subtype);

#endif
