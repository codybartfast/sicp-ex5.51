#ifndef OBJ_H
#define OBJ_H
#include "sicpstd.h"

#include <stdbool.h>
#include <inttypes.h>

#define OBJ_4(TYPE, STYPE, VTYPE, VALUE)                                       \
	{                                                                      \
		TYPE, STYPE,                                                   \
		{                                                              \
			.VTYPE = VALUE                                         \
		}                                                              \
	}

enum type {
	TYPE_NOT_SET = 0,
	TYPE_SYMBOL, // 1
	TYPE_BOOL, // 2
	TYPE_NUMBER, // 3
	TYPE_STRING, // 4
	TYPE_EMPTY_LIST, // 5
	TYPE_PAIRPTR, // 6

	TYPE_EOF, // 7
	TYPE_PRIMITIVE_PROCEDURE, // 8
	TYPE_UNSPECIFIED, // 9
	TYPE_VOID, // 10
	TYPE_BROKEN_HEART, // 11
	TYPE_BITMAP, // 12
	TYPE_UNASSIGNED, // 13
	TYPE_ERROR // 14
};

enum { SUBTYPE_NOT_SET = 0 };

enum { BOOL_TRUE = 1, BOOL_FALSE = 2 };
enum { NUMBER_INTEGER = 1, NUMBER_FLOATING };

typedef struct obj {
	uint8_t type;
	uint8_t subtype;
	union {
		Integer Integer;
		Floating Floating;
		const char *string;
		struct pair *reference;
		struct obj (*primproc)(struct obj);
		const struct bitmap *bitmap;
	} val;
} obj;

struct pair {
	struct obj car;
	struct obj cdr;
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
extern const obj true_o;
extern const obj false_o;
extern const obj true_s;
extern const obj false_s;

// NUMBER

bool is_number(obj);
obj of_integer(Integer);
Integer to_integer(obj dat);
obj of_floating(Floating);
Floating to_floating(obj);
extern const obj one;
extern const obj zero;

// STRINGS

bool is_string(obj);
extern const obj emptystr;
extern const obj nl;

obj of_string(const char *str);
const char *to_string(const obj dat);

// PAIRS

bool is_pair(obj);
bool is_null(obj dat);
obj of_pairptr(struct pair *);
struct pair *to_pairptr(obj);

extern const obj emptylst;
obj cons(obj, obj);
obj consgc(obj *, obj *);
obj car(obj);
obj set_car(obj, obj);
obj cdr(obj);
obj set_cdr(obj, obj);

// BITMAP

bool is_bitmap(obj);
const struct bitmap *to_bitmap(const obj);

// PRIMITIVE PROCEDURES

bool is_primitive_procedure(obj dat);
obj of_function(obj (*funptr)(obj));
obj (*to_function(obj dat))(obj);

// KEYWORDS

extern const obj amb;
extern const obj amb_fail;
extern const obj and_s;
extern const obj uapply;
extern const obj __ppapply;
extern const obj arrow;
extern const obj begin;
extern const obj cond;
extern const obj cons_s;
extern const obj cons_stream;
extern const obj define;
extern const obj delay;
extern const obj else_s;
extern const obj failed;
extern const obj if_s;
extern const obj let;
extern const obj letrec;
extern const obj letstar;
extern const obj lambda;
extern const obj memo_proc;
extern const obj or_s;
extern const obj quote;
extern const obj set;
extern const obj time_s;

// EVAL TAGS
extern const obj procedure;

// ECEVAL LABELS
extern const obj ev_appl_accum_last_arg;
extern const obj ev_appl_accumulate_arg;
extern const obj ev_appl_did_operator;
extern const obj ev_apply_2;
extern const obj ev_apply_3;
extern const obj ev_assignment_1;
extern const obj ev_definition_1;
extern const obj ev_if_decide;
extern const obj ev_quoted;
extern const obj ev_sequence_continue;
extern const obj ev_timed_done;
extern const obj ev_return_caller;

// MISC VALUES

bool is_eof(obj dat);
extern const obj eof;
extern const obj pex;
extern const obj ok;
extern const obj unspecified;
bool is_void(obj dat);
extern const obj void_o;

// BROKEN HEART

bool is_broken_heart(obj dat);
extern const obj broken_heart;

// UNASSIGNED

bool is_unassigned_obj(obj dat);
extern const obj unassigned;

// ERROR

bool is_err(obj obj);
obj make_err(int err_subtype);

#endif
