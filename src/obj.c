#include <stdlib.h>
#include <string.h>
#include "obj.h"
#include "error.h"

#define AREA "OBJ"
#define VALUE val /* allow renaming of value member to check api leak */

inline static obj new_simp(int type, int subtype)
{
	obj dat = { false, .simp = { .type = type, .subtype = subtype } };
	return dat;
}

// EMPTY LIST

const obj the_empty_list = {
	false, .simp = { TYPE_EMPTY_LIST, SUBTYPE_NOT_SET, { 0 } }
};

bool isnull(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_EMPTY_LIST;
}

static obj tel(void)
{
	return the_empty_list;
}

// REFERENCE

inline static bool isreference(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_REFERENCE;
}

static obj reference(obj dat)
{
	obj *ptr = (obj *)calloc(sizeof(obj), 1);
	*ptr = dat;
	obj ref = new_simp(TYPE_REFERENCE, SUBTYPE_NOT_SET);
	ref.simp.val.pointer = ptr;
	return ref;
}

static obj dereference(obj dat)
{
	return *dat.simp.VALUE.pointer;
}

// PAIR

static inline bool ispair(obj dat)
{
	return dat.ispair;
}

obj cons(obj car, obj cdr)
{
	obj p = { true,
		  .pair = { ispair(car) ? reference(car).simp : car.simp,
			    ispair(cdr) ? reference(cdr).simp : cdr.simp } };
	return p;
}

obj car(obj pair)
{
	obj dat = { false, .simp = pair.pair.car };
	if (!ispair(pair)) {
		eprintf(AREA, "car expects a pair");
		return error_argument_type();
	}
	return isreference(dat) ? dereference(dat) : dat;
}

obj cdr(obj pair)
{
	obj dat = { false, .simp = pair.pair.cdr };
	if (!ispair(pair)) {
		eprintf(AREA, "cdr expects a pair");
		return error_argument_type();
	}
	return isreference(dat) ? dereference(dat) : dat;
}

obj cdr(obj pair);
obj set_car(obj pair);
obj set_cdr(obj pair);

// NUMBER

inline static bool isnumber(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_NUMBER;
}

static obj ofint64(int64_t n)
{
	obj dat = new_simp(TYPE_NUMBER, NUMBER_INT64);
	dat.simp.VALUE.int64 = n;
	return dat;
}

static int64_t toint64(obj dat)
{
	return dat.simp.VALUE.int64;
}

// STRING

inline bool isstring(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_STRING;
}

const obj nl_struct = {
	false, .simp = { TYPE_STRING, SUBTYPE_NOT_SET, { .string = "\n" } }
};

static obj nl(void)
{
	return nl_struct;
}

static obj ofstring(char *str)
{
	obj dat = new_simp(TYPE_STRING, SUBTYPE_NOT_SET);
	dat.simp.VALUE.string = str;
	return dat;
}

static char *tostring(obj dat)
{
	return dat.simp.VALUE.string;
}

// UNSPECIFIED

const obj unspecified_struct = {
	false, .simp = { TYPE_UNSPECIFIED, SUBTYPE_NOT_SET, { 0 } }
};

static obj unspecified(void)
{
	return unspecified_struct;
}

// EOF

const obj eof_struct = { false, .simp = { TYPE_EOF, SUBTYPE_NOT_SET, { 0 } } };

static bool iseof(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_EOF;
}

static obj eof(void)
{
	return eof_struct;
}

// ERROR

obj make_err(int err_subtype)
{
	obj obj = new_simp(TYPE_ERROR, err_subtype);
	return obj;
}

// Accessor

const struct obj_accessor Obj = { 
				  .tel = tel,

				  .isreference = isreference,
				  .reference = reference,
				  .dereference = dereference,

				  .ispair = ispair,

				  .isnumber = isnumber,
				  .ofint64 = ofint64,
				  .toint64 = toint64,

				  .nl = nl,
				  .ofstring = ofstring,
				  .tostring = tostring,

				  .unspecified = unspecified,

				  .iseof = iseof,
				  .eof = eof,

				  .iserr = iserr };
