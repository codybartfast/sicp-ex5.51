#ifndef OBJ_H
#define OBJ_H

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

enum subtype { SUBTYPE_NOT_SET = 0, NUMBER_INT64 };

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

enum type type(obj);
enum subtype subtype(obj);

bool issymbol(obj);
bool isnumber(obj);
bool isstring(obj);
bool ispair(obj);
bool isnull(obj dat);
bool isreference(obj dat);
bool isprimproc(obj dat);
bool iseof(obj dat);

extern const obj emptylst;
obj cons(obj, obj);
obj car(obj);
obj cdr(obj);

struct obj_accessor {
	obj (*ofidentifier)(char *);
	char *(*toidentifier)(obj);

	obj (*ofint64)(int64_t);
	int64_t (*toint64)(obj);

	obj (*nl)(void);
	obj (*ofstring)(char *);
	char *(*tostring)(obj);

	obj (*reference)(obj);
	obj (*dereference)(obj);

	obj (*offunction)(obj (*)(obj));
	obj (*(*tofunction)(obj))(obj);

	obj (*unspecified)(void);

	obj (*eof)(void);
};

extern const struct obj_accessor Obj;

obj make_err(int err_subtype);

#endif
