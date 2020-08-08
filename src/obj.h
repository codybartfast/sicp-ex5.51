#ifndef OBJ_H
#define OBJ_H 1

#include <stdbool.h>
#include <inttypes.h>

#define TYPE_NOT_SET 0
#define TYPE_EMPTY_LIST 1
#define TYPE_REFERENCE 2
#define TYPE_NUMBER 3
#define TYPE_STRING 4
#define TYPE_UNSPECIFIED 6
#define TYPE_EOF 7
#define TYPE_ERROR 8

#define SUBTYPE_NOT_SET 0
#define NUMBER_INT64 1

struct obj_struct;

struct simp {
	uint8_t type;
	uint8_t subtype;
	union {
		struct obj_struct *pointer;
		int64_t int64;
		char *string;
	} val;
};

struct pair {
	struct simp car;
	struct simp cdr;
};

typedef struct obj_struct {
	bool ispair;
	union {
		struct simp simp;
		struct pair pair;
	};
} obj;

bool isnull(obj dat);

obj cons(obj, obj);
obj car(obj);
obj cdr(obj);
obj set_car(obj);
obj set_cdr(obj);

struct obj_accessor {
	obj (*tel)(void);

	bool (*isreference)(obj);
	obj (*reference)(obj);
	obj (*dereference)(obj);

	bool (*ispair)(obj);

	bool (*isnumber)(obj);
	obj (*ofint64)(int64_t);
	int64_t (*toint64)(obj);

	bool (*isstring)(obj);
	obj (*nl)(void);
	obj (*ofstring)(char *);
	char *(*tostring)(obj);

	obj (*unspecified)(void);

	bool (*iseof)(obj);
	obj (*eof)(void);

	bool (*iserr)(obj);
};

extern const struct obj_accessor Obj;

obj make_err(int err_subtype);

#endif
