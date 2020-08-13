#include "primproc.h"

#include "error.h"
#include "output.h"
#include "list.h"

#define AREA "PRIMPROC"

obj add_pp(obj args)
{
	int64_t acc = 0;
	do {
		obj fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "+ given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc += Obj.to_int64(fst);
	} while (is_pair(args = cdr(args)));
	return Obj.of_int64(acc);
}

obj sub_pp(obj args)
{
	int64_t acc;
	obj fst;
	if (!is_pair(args)) {
		eprintf(AREA, "'-' given no arguments");
		return error_argument_type();
	}
	fst = car(args);
	if (!is_number(fst)) {
		eprintf(AREA, "'-' given non-number: %s",
			Obj.to_string(writestr(fst)));
		return error_argument_type();
	}
	acc = Obj.to_int64(fst);
	acc = is_null(cdr(args)) ? -acc : acc;
	while (is_pair(args = cdr(args))) {
		fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "'-' given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc -= Obj.to_int64(fst);
	}
	return Obj.of_int64(acc);
}

obj mul_pp(obj args)
{
	int64_t acc = 1;
	do {
		obj fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "* given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc *= Obj.to_int64(fst);
	} while (is_pair(args = cdr(args)));
	return Obj.of_int64(acc);
}

obj div_pp(obj args)
{
	int64_t acc;
	obj fst;
	if (!is_pair(args)) {
		eprintf(AREA, "'/' given no arguments");
		return error_argument_type();
	}
	fst = car(args);
	if (!is_number(fst)) {
		eprintf(AREA, "'/' given non-number: %s",
			Obj.to_string(writestr(fst)));
		return error_argument_type();
	}
	acc = Obj.to_int64(fst);
	acc = is_null(cdr(args)) ? (1 / acc) : acc;
	while (is_pair(args = cdr(args))) {
		fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "'/' given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc /= Obj.to_int64(fst);
	}
	return Obj.of_int64(acc);
}