#ifndef OBJ_H
#define OBJ_H
#include "sicpstd.h"

#include <stdbool.h>
#include <inttypes.h>

enum type {
	TYPE_NOT_SET = 0,
	TYPE_SYMBOL,
	TYPE_NUMBER,
	TYPE_STRING,
	TYPE_EMPTY_LIST,
	TYPE_REFERENCE,
	TYPE_PRIMITIVE_PROCEDURE,
	TYPE_UNSPECIFIED,
	TYPE_EOF,
	TYPE_ERROR
};

enum { SUBTYPE_NOT_SET = 0 };

enum { NUMBER_INT64 = 1, NUMBER_DOUBLE };

struct obj;

struct simp {
	uint8_t type;
	uint8_t subtype;
	union {
		int64_t int64;
		double iec559;
		char *string;
		struct obj *reference;
		struct obj (*primproc)(struct obj);
	} val;
};

struct pair {
	struct simp car;
	struct simp cdr;
};

typedef struct obj {
	bool is_pair;
	union {
		struct simp simp;
		struct pair pair;
	};
} obj;

enum type type(obj);
int subtype(obj);

bool is_symbol(obj);
bool is_number(obj);
bool is_string(obj);
bool is_pair(obj);
bool is_null(obj dat);
bool is_reference(obj dat);
bool is_primproc(obj dat);
bool is_eof(obj dat);

// NUMBER

obj of_integer(int64_t);
int64_t to_integer(obj dat);
obj of_double(double);
double to_double(obj);

extern const obj emptylst;
obj cons(obj, obj);
obj car(obj);
obj cdr(obj);

struct obj_accessor {
	obj (*of_identifier)(char *);
	char *(*to_identifier)(obj);

	obj (*nl)(void);
	obj (*of_string)(char *);
	char *(*to_string)(obj);

	obj (*reference)(obj);
	obj (*dereference)(obj);

	obj (*of_function)(obj (*)(obj));
	obj (*(*to_function)(obj))(obj);

	obj (*unspecified)(void);

	obj (*eof)(void);
};

extern const struct obj_accessor Obj;

obj make_err(int err_subtype);

#endif
