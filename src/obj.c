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

// EOF

const obj eof_struct = { false, .simp = { TYPE_EOF, 0, { 0 } } };

static bool iseof(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_EOF;
}

static obj eof(void)
{
	return eof_struct;
}

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

inline static bool isstring(obj dat)
{
	return !dat.ispair && dat.simp.type == TYPE_STRING;
}

const obj newline_struct = {
	false, .simp = { TYPE_STRING, SUBTYPE_NOT_SET, { .string = "\n" } }
};

static obj newline(void)
{
	return newline_struct;
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

// Accessor

const struct obj_accessor Obj = { .iserr = iserr,
				  .iseof = iseof,
				  .eof = eof,
				  .isnumber = isnumber,
				  .ofint64 = ofint64,
				  .toint64 = toint64,
				  .isstring = isstring,
				  .newline = newline,
				  .ofstring = ofstring,
				  .tostring = tostring };

// ERROR

obj make_err(int err_subtype)
{
	obj obj = new_simp(TYPE_ERROR, err_subtype);
	return obj;
}
