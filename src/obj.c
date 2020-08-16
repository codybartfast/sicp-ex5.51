#include "obj.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"

#define AREA "OBJ"

static obj new_simp(int type, int subtype)
{
	obj dat = { false, .simp = { .type = type, .subtype = subtype } };
	return dat;
}

enum type type(obj dat)
{
	if (dat.is_pair) {
		error_argument_type(AREA, "Cannot get type of a pair");
		return -1;
	}
	return dat.simp.type;
}

int subtype(obj dat)
{
	if (dat.is_pair) {
		error_argument_type(AREA, "Cannot get type of a pair");
		return -1;
	}
	return dat.simp.subtype;
}

// ERROR

bool is_err(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_ERROR;
}

obj make_err(int err_subtype)
{
	obj obj = new_simp(TYPE_ERROR, err_subtype);
	return obj;
}

// SYMBOL

bool is_symbol(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_SYMBOL;
}

obj of_identifier(const char *id)
{
	obj dat = new_simp(TYPE_SYMBOL, SUBTYPE_NOT_SET);
	dat.simp.val.string = id;
	return dat;
}

// NUMBER

bool is_number(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_NUMBER;
}

obj of_integer(INTEGER n)
{
	obj dat = new_simp(TYPE_NUMBER, NUMBER_INTEGER);
	dat.simp.val.integer = n;
	return dat;
}

INTEGER to_integer(obj dat)
{
	return dat.simp.val.integer;
}

obj of_floating(FLOATING n)
{
	obj dat = new_simp(TYPE_NUMBER, NUMBER_FLOATING);
	dat.simp.val.floating = n;
	return dat;
}

FLOATING to_floating(obj dat)
{
	return dat.simp.val.floating;
}

const obj zero = { false,
		   .simp = { TYPE_NUMBER, NUMBER_INTEGER, { .integer = 0 } } };
const obj one = { false,
		  .simp = { TYPE_NUMBER, NUMBER_INTEGER, { .integer = 1 } } };

// STRING

bool is_string(const obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_STRING;
}

const obj nl = { false,
		 .simp = { TYPE_STRING, SUBTYPE_NOT_SET, { .string = "\n" } } };

obj of_string(const char *str)
{
	obj dat = { false, .simp = { .type = TYPE_STRING,
				     .subtype = SUBTYPE_NOT_SET,
				     { .string = str } } };
	return dat;
}

const char *to_string(const obj dat)
{
	return dat.simp.val.string;
}

// PAIR

bool is_pair(obj dat)
{
	return type(dat) == TYPE_REFERENCE;
}

bool is_null(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_EMPTY_LIST;
}

const obj emptylst = { false,
		       .simp = { TYPE_EMPTY_LIST, SUBTYPE_NOT_SET, { 0 } } };

obj cons(obj car, obj cdr)
{
	obj pair = { true, .pair = { car.simp, cdr.simp } };
	obj *ptr = (obj *)calloc(sizeof(obj), 1);
	if (ptr == NULL) {
		eprintf(AREA, "No memory for reference");
		return error_memory();
	}
	*ptr = pair;
	obj ref = { false, .simp = { TYPE_REFERENCE,
				     SUBTYPE_NOT_SET,
				     { .reference = ptr } } };
	return ref;
}

obj car(obj pair)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "car expects a pair");
	} else {
		obj dat = { false, .simp = pair.simp.val.reference->pair.car };
		return dat;
	}
}

obj set_car(obj *pair, obj val)
{
	if (!is_pair(*pair)) {
		return error_argument_type(AREA, "set_car expects a pair");
	} else {
		pair->simp.val.reference->pair.car = val.simp;
		return unspecified;
	}
}

obj cdr(obj pair)
{
	if (!is_pair(pair)) {
		return error_argument_type(AREA, "cdr expects a pair");
	} else {
		obj dat = { false, .simp = pair.simp.val.reference->pair.cdr };
		return dat;
	}
}

obj set_cdr(obj *pair, obj val)
{
	if (!is_pair(*pair)) {
		return error_argument_type(AREA, "set_cdr expects a pair");
	} else {
		pair->simp.val.reference->pair.cdr = val.simp;
		return unspecified;
	}
}

// PRIMITIVE PROCEDURES (FUNCTIONS)

bool is_primproc(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_PRIMITIVE_PROCEDURE;
}

obj of_function(obj (*funptr)(obj))
{
	obj pp = new_simp(TYPE_PRIMITIVE_PROCEDURE, SUBTYPE_NOT_SET);
	pp.simp.val.primproc = funptr;
	return pp;
}

obj (*to_function(obj dat))(obj)
{
	return dat.simp.val.primproc;
}

// KEYWORDS

const obj define = {
	false, .simp = { TYPE_SYMBOL, SUBTYPE_NOT_SET, { .string = "define" } }
};

const obj lambda = {
	false, .simp = { TYPE_SYMBOL, SUBTYPE_NOT_SET, { .string = "lambda" } }
};

// MISC VALUES

bool is_eof(obj dat)
{
	return !is_pair(dat) && dat.simp.type == TYPE_EOF;
}

const obj eof = { false, .simp = { TYPE_EOF, SUBTYPE_NOT_SET, { 0 } } };

const obj unspecified = {
	false, .simp = { TYPE_UNSPECIFIED, SUBTYPE_NOT_SET, { 0 } }
};

const obj ok = { false,
		 .simp = { TYPE_SYMBOL, SUBTYPE_NOT_SET, { .string = "ok" } } };
