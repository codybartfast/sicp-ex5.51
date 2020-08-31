#include "obj.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "storage.h"

#define OBJ_4(TYPE, STYPE, VTYPE, VALUE)                                       \
	{                                                                      \
		TYPE, STYPE,                                                   \
		{                                                              \
			.VTYPE = VALUE                                         \
		}                                                              \
	}

#define OBJ_2(TYPE, STYPE)                                                     \
	{                                                                      \
		TYPE, STYPE,                                                   \
		{                                                              \
			0                                                      \
		}                                                              \
	}

#define SYMBOL(NAME)                                                           \
	const obj NAME = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, #NAME);

#define AREA "OBJ"

enum type type(obj dat)
{
	return dat.type;
}

int subtype(obj dat)
{
	return dat.subtype;
}

// SYMBOL

bool is_symbol(obj dat)
{
	return type(dat) == TYPE_SYMBOL;
}

obj of_identifier(const char *id)
{
	return (obj)OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, id);
}

// BOOLS

bool is_boolean(obj dat)
{
	return type(dat) == TYPE_BOOL;
}

bool is_true(obj dat)
{
	return !is_false(dat);
}

bool is_false(obj dat)
{
	return type(dat) == TYPE_BOOL && subtype(dat) == BOOL_FALSE;
}

const obj tru_o = OBJ_2(TYPE_BOOL, BOOL_TRUE);
const obj fls_o = OBJ_2(TYPE_BOOL, BOOL_FALSE);
const obj tru = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "true");
const obj fls = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "false");

// NUMBER

bool is_number(obj dat)
{
	return type(dat) == TYPE_NUMBER;
}

obj of_integer(Integer n)
{
	return (obj)OBJ_4(TYPE_NUMBER, NUMBER_INTEGER, Integer, n);
}

Integer to_integer(obj dat)
{
	return dat.val.Integer;
}

obj of_floating(Floating n)
{
	return (obj)OBJ_4(TYPE_NUMBER, NUMBER_FLOATING, Floating, n);
}

Floating to_floating(obj dat)
{
	return dat.val.Floating;
}

const obj zero = OBJ_4(TYPE_NUMBER, NUMBER_INTEGER, Integer, 0);

const obj one = OBJ_4(TYPE_NUMBER, NUMBER_INTEGER, Integer, 1);

// STRING

bool is_string(const obj dat)
{
	return type(dat) == TYPE_STRING;
}

const obj emptystr = OBJ_4(TYPE_STRING, SUBTYPE_NOT_SET, string, "");
const obj nl = OBJ_4(TYPE_STRING, SUBTYPE_NOT_SET, string, "\n");

obj of_string(const char *str)
{
	return (obj)OBJ_4(TYPE_STRING, SUBTYPE_NOT_SET, string, str);
}

const char *to_string(const obj dat)
{
	return dat.val.string;
}

// PAIR

bool is_pair(obj dat)
{
	return type(dat) == TYPE_REFERENCE;
}

bool is_null(obj dat)
{
	return type(dat) == TYPE_EMPTY_LIST;
}

const obj emptylst = OBJ_2(TYPE_EMPTY_LIST, SUBTYPE_NOT_SET);

static obj consnogc(obj car, obj cdr, bool nogc)
{
	struct cell *ptr = newcell(nogc);
	if (ptr == NULL) {
		return error_memory(AREA, "Reference");
	}
	*ptr = (struct cell){ .pair = { car, cdr } };
	return (obj)OBJ_4(TYPE_REFERENCE, SUBTYPE_NOT_SET, reference, ptr);
}

obj pcons(obj car, obj cdr)
{
	return consnogc(car, cdr, true);
}

obj cons(obj car, obj cdr)
{
	return consnogc(car, cdr, false);
}

obj car(obj pair)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "car expects a pair");
	} else {
		return pair.val.reference->pair.car;
	}
}

obj set_car(obj pair, obj val)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "set_car expects a pair");
	} else {
		pair.val.reference->pair.car = val;
		return unspecified;
	}
}

obj cdr(obj pair)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "cdr expects a pair");
	} else {
		return pair.val.reference->pair.cdr;
	}
}

obj set_cdr(obj pair, obj val)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "set_cdr expects a pair");
	} else {
		pair.val.reference->pair.cdr = val;
		return unspecified;
	}
}

// PRIMITIVE PROCEDURES

bool is_primitive_procedure(obj dat)
{
	return type(dat) == TYPE_PRIMITIVE_PROCEDURE;
}

obj of_function(obj (*funptr)(obj))
{
	return (obj)OBJ_4(TYPE_PRIMITIVE_PROCEDURE, SUBTYPE_NOT_SET, primproc,
			  funptr);
}

obj (*to_function(obj dat))(obj)
{
	return dat.val.primproc;
}

// KEYWORDS

SYMBOL(begin)
SYMBOL(cond)
SYMBOL(define)
const obj else_s = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "else");
const obj if_s = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "if");
SYMBOL(let)
SYMBOL(lambda)
const obj time_s = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "time");

// TAGS
SYMBOL(procedure)

// ECEVAL LABELS
SYMBOL(ev_appl_accum_last_arg)
SYMBOL(ev_appl_did_operator)
SYMBOL(ev_appl_accumulate_arg)
SYMBOL(ev_assignment_1)
SYMBOL(ev_definition_1)
SYMBOL(ev_if_decide)
SYMBOL(ev_sequence_continue)
SYMBOL(ev_timed_done)
SYMBOL(return_caller)

// MISC VALUES

bool is_eof(obj dat)
{
	return type(dat) == TYPE_EOF;
}
const obj eof = OBJ_2(TYPE_EOF, SUBTYPE_NOT_SET);

const obj _ex = OBJ_4(TYPE_SYMBOL, SUBTYPE_NOT_SET, string, "%ex");

SYMBOL(ok)

const obj unspecified = OBJ_2(TYPE_UNSPECIFIED, SUBTYPE_NOT_SET);

bool is_void(obj dat)
{
	return type(dat) == TYPE_VOID;
}
const obj _void = OBJ_2(TYPE_VOID, SUBTYPE_NOT_SET);

// ERROR

bool is_err(obj dat)
{
	return type(dat) == TYPE_ERROR;
}

obj make_err(int err_subtype)
{
	return (obj)OBJ_2(TYPE_ERROR, err_subtype);
}
