#ifndef OBJ_H
#define OBJ_H
#include "sicpstd.h"

#include <stdbool.h>
#include <inttypes.h>

enum type {
	TYPE_NOT_SET = 0,
	TYPE_SYMBOL, // 1
	TYPE_BOOL, // 2
	TYPE_NUMBER, // 3
	TYPE_STRING, // 4
	TYPE_EMPTY_LIST, // 5
	TYPE_REFERENCE, // 6
	TYPE_PRIMITIVE_PROCEDURE, // 7
	TYPE_UNSPECIFIED, // 8
	TYPE_EOF, // 9
	TYPE_ERROR // 10
};

enum { SUBTYPE_NOT_SET = 0 };

enum { BOOL_TRUE = 1, BOOL_FALSE = 2 };
enum { NUMBER_INTEGER = 1, NUMBER_FLOATING };

typedef struct obj {
	uint8_t type;
	uint8_t subtype;
	union {
		INTEGER integer;
		FLOATING floating;
		const char *string;
		struct cell *reference;
		struct obj (*primproc)(struct obj);
	} val;
} obj;

struct pair {
	struct obj car;
	struct obj cdr;
};

struct cell {
	bool is_pair;
	union {
		struct obj object;
		struct pair pair;
	};
};

enum type type(obj);
int subtype(obj);

// SYMBOL

bool is_symbol(obj);
obj of_identifier(const char *id);

// BOOLS
bool is_boolean(obj);
bool is_true(obj);
bool is_false(obj);
extern const obj tru_o;
extern const obj fls_o;
extern const obj tru;
extern const obj fls;

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

extern const obj emptylst;
obj cons(obj, obj);
obj car(obj);
obj set_car(obj, obj);
obj cdr(obj);
obj set_cdr(obj, obj);

// PRIMITIVE PROCEDURES

bool is_primproc(obj dat);
obj of_function(obj (*funptr)(obj));
obj (*to_function(obj dat))(obj);

// KEYWORDS

extern const obj define;
extern const obj if_s;
extern const obj lambda;

// EVAL TAGS
extern const obj procedure;

// MISC VALUES

bool is_eof(obj dat);
extern const obj eof;
extern const obj unspecified;
extern const obj ok;

// ERROR

bool is_err(obj obj);
obj make_err(int err_subtype);

#endif
