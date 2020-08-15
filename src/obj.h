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

enum { NUMBER_INTEGER = 1, NUMBER_FLOATING };

struct obj;

struct simp {
	uint8_t type;
	uint8_t subtype;
	union {
		INTEGER integer;
		FLOATING floating;
		const char *string;
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

obj make_err(int err_subtype);

// SYMBOL

bool is_symbol(obj);
obj of_identifier(char *id);

// NUMBER

bool is_number(obj);
obj of_integer(INTEGER);
INTEGER to_integer(obj dat);
obj of_floating(FLOATING);
FLOATING to_floating(obj);
extern const obj one;
extern const obj zero;

// STRINGS

bool is_string(obj);
extern const obj nl;

obj of_string(const char *str);
const char *to_string(const obj dat);

// PAIRS

bool is_pair(obj);
bool is_null(obj dat);
bool is_reference(obj dat);

extern const obj emptylst;
obj cons(obj, obj);
obj car(obj);
obj cdr(obj);

// PRIMITIVE PROCEDURES

bool is_primproc(obj dat);
obj of_function(obj (*funptr)(obj));
obj (*to_function(obj dat))(obj);

// MISC VALUES

bool is_eof(obj dat);
extern const obj eof;
extern const obj unspecified;

#endif
