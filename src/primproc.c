#include "primproc.h"

#include "stdbool.h"
#include "error.h"
#include "output.h"
#include "list.h"

#define AREA "PRIMPROC"

static bool is_zero(obj n)
{
	switch (subtype(n)) {
	case NUMBER_INTEGER:
		return to_integer(n) == 0;
	case NUMBER_FLOATING:
		return to_double(n) == 0;
	default:
		eprintf(AREA, "BUG! No is_zero case for: %d", subtype(n));
		return true;
	}
}

static obj chknums(char *fn, obj args)
{
	obj n, orig = args;
	int i = 0;
	int inex = -1;
	int zero = -1;

	for (; is_pair(args); args = cdr(args), i++) {
		n = car(args);
		if (!is_number(n)) {
			eprintf(AREA, "primitive %s given non number: %s", fn,
				"blah"); //errstr(n));
			return error_argument_type();
		}
		if (subtype(n) != NUMBER_INTEGER && inex == -1)
			inex = i;
		if (is_zero(n) && zero == -1)
			zero = i;
	}
	if (!is_null(args)) {
		eprintf(AREA, "%s not given a proper list: %s", fn,
			"blah"); //errstr(orig));
		return error_argument_type();
	}

	return list2(of_integer(inex), of_integer(zero));
}

static int chknums_inex(obj r)
{
	return to_integer(car(r));
}

static int chknums_zero(obj r)
{
	return to_integer(cadr(r));
}

static int64_t add_pp_int64(obj args)
{
	int64_t acc;

	for (acc = 0; is_pair(args); args = cdr(args))
		acc += to_integer(car(args));
	return acc;
}

static double add_pp_double(obj args)
{
	double acc;
	obj n;

	for (acc = 0; is_pair(args); args = cdr(args)) {
		n = car(args);
		acc += (subtype(n) == NUMBER_FLOATING) ? to_double(n) :
							 to_integer(n);
	}
	return acc;
}

obj add_pp(obj args)
{
	obj chk = chknums("add", args);
	if (is_err(chk))
		return chk;
	return (chknums_inex(chk) > -1) ? of_double(add_pp_double(args)) :
					  of_integer(add_pp_int64(args));
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
	acc = to_integer(fst);
	acc = is_null(cdr(args)) ? -acc : acc;
	while (is_pair(args = cdr(args))) {
		fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "'-' given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc -= to_integer(fst);
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
		acc *= to_integer(fst);
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
	acc = to_integer(fst);
	acc = is_null(cdr(args)) ? (1 / acc) : acc;
	while (is_pair(args = cdr(args))) {
		fst = car(args);
		if (!is_number(fst)) {
			eprintf(AREA, "'/' given non-number: %s",
				Obj.to_string(writestr(fst)));
			return error_argument_type();
		}
		acc /= to_integer(fst);
	}
	return of_integer(acc);
}
