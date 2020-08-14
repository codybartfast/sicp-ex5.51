#include "primproc.h"

#include "error.h"
#include "output.h"
#include "list.h"

#define AREA "PRIMPROC"

// overflow?
static int64_t add_pp_int64(obj args)
{
	int64_t acc;

	for (acc = 0; is_pair(args); args = cdr(args))
		acc += Obj.to_int64(car(args));
	return acc;
}

static double add_pp_double(obj args)
{
	double acc;
	obj n;

	for (acc = 0; is_pair(args); args = cdr(args)) {
		n = car(args);
		acc += (subtype(n) == NUMBER_DOUBLE) ? to_double(n) :
						       Obj.to_int64(n);
	}
	return acc;
}

obj add_pp(obj args)
{
	bool gotfloat = false;
	obj lst;
	for (lst = args; is_pair(lst); lst = cdr(lst)) {
		obj itm = car(lst);
		if (!is_number(itm)) {
			eprintf(AREA, "+ given non-number: %s",
				Obj.to_string(writestr(itm)));
			return error_argument_type();
		}
		gotfloat |= (subtype(itm) == NUMBER_DOUBLE);
	}
	if (!is_null(lst)) {
		eprintf(AREA, "+ given inmproper list");
		return error_argument_type();
	}
	if (!gotfloat) {
		return of_integer(add_pp_int64(args));
	}
	return of_double(add_pp_double(args));
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
	return of_integer(acc);
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
	return of_integer(acc);
}

// div by zero?
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
	return of_integer(acc);
}
