#include "obj.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"

#define CONST_OBJ(NAME, TYPE, STYPE, VTYPE, VALUE)                             \
	const obj NAME = { TYPE, STYPE, { .VTYPE = VALUE } };

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

#define OBJ_1(TYPE) OBJ_2(TYPE, SUBTYPE_NOT_SET)

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

// ERROR

bool is_err(obj dat)
{
	return type(dat) == TYPE_ERROR;
}

obj make_err(int err_subtype)
{
	return (obj)OBJ_2(TYPE_ERROR, err_subtype);
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

// NUMBER

bool is_number(obj dat)
{
	return type(dat) == TYPE_NUMBER;
}

obj of_integer(INTEGER n)
{
	return (obj)OBJ_4(TYPE_NUMBER, NUMBER_INTEGER, integer, n);
}

INTEGER to_integer(obj dat)
{
	return dat.val.integer;
}

obj of_floating(FLOATING n)
{
	return (obj)OBJ_4(TYPE_NUMBER, NUMBER_FLOATING, floating, n);
}

FLOATING to_floating(obj dat)
{
	return dat.val.floating;
}

CONST_OBJ(zero, TYPE_NUMBER, NUMBER_INTEGER, integer, 0)

CONST_OBJ(one, TYPE_NUMBER, NUMBER_INTEGER, integer, 1)

// STRING

bool is_string(const obj dat)
{
	return type(dat) == TYPE_STRING;
}

CONST_OBJ(nl, TYPE_STRING, SUBTYPE_NOT_SET, string, "\n")

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

const obj emptylst = OBJ_1(TYPE_EMPTY_LIST);

obj cons(obj car, obj cdr)
{
	struct cell *ptr = (struct cell *)calloc(sizeof(struct cell), 1);
	if (ptr == NULL) {
		return error_memory(AREA, "Reference");
	}
	*ptr = (struct cell){ true, .pair = { car, cdr } };
	return (obj)OBJ_4(TYPE_REFERENCE, SUBTYPE_NOT_SET, reference, ptr);
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

// PRIMITIVE PROCEDURES (FUNCTIONS)

bool is_primproc(obj dat)
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

SYMBOL(define)

SYMBOL(lambda)

// MISC VALUES

bool is_eof(obj dat)
{
	return type(dat) == TYPE_EOF;
}

const obj eof = OBJ_1(TYPE_EOF);

const obj unspecified = OBJ_1(TYPE_UNSPECIFIED);

SYMBOL(ok)
