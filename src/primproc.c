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
		return to_floating(n) == 0;
	default:
		eprintf(AREA, "BUG! No is_zero case for: %d", subtype(n));
		return true;
	}
}

static FLOATING num_to_floating(obj n)
{
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		return to_floating(n);
	case NUMBER_INTEGER:
		return to_integer(n);
	default:
		eprintf(AREA, "BUG! No num_tofloating case for: %d",
			subtype(n));
		return -1;
	}
}

static obj chknums(char *fn, obj args)
{
	obj n, orig = args;
	int i = 0;
	int fltcnt = -1;
	int zerocnt = -1;

	for (; is_pair(args); args = cdr(args), i++) {
		n = car(args);
		if (!is_number(n)) {
			eprintf(AREA, "primitive %s given non number: %s", fn,
				errstr(n));
			return error_argument_type();
		}
		if (subtype(n) != NUMBER_INTEGER && fltcnt == -1)
			fltcnt = i;
		if (is_zero(n) && zerocnt == -1)
			zerocnt = i;
	}
	if (!is_null(args)) {
		eprintf(AREA, "%s not given a proper list: %s", fn,
			errstr(orig));
		return error_argument_type();
	}
	return list3(of_integer(i), of_integer(fltcnt), of_integer(zerocnt));
}

static int chknums_len(obj r)
{
	return to_integer(car(r));
}

static int chknums_flt(obj r)
{
	return to_integer(cadr(r));
}

static int chknums_zero(obj r)
{
	return to_integer(caddr(r));
}

static obj add_pp_integer(obj args)
{
	INTEGER acc;

	for (acc = 0; is_pair(args); args = cdr(args))
		acc += to_integer(car(args));
	return of_integer(acc);
}

static obj add_pp_floating(obj args)
{
	FLOATING acc;
	obj n;

	for (acc = 0; is_pair(args); args = cdr(args)) {
		n = car(args);
		acc += num_to_floating(n);
	}
	return of_floating(acc);
}

obj add_pp(obj args)
{
	obj chk = chknums("add", args);
	if (is_err(chk))
		return chk;
	return (chknums_flt(chk) > -1) ? add_pp_floating(args) :
					 add_pp_integer(args);
}

static obj sub_pp_integer(obj args)
{
	INTEGER acc = to_integer(car(args));

	for (args = cdr(args); is_pair(args); args = cdr(args))
		acc -= to_integer(car(args));
	return of_integer(acc);
}

static obj sub_pp_floating(obj args)
{
	FLOATING acc = num_to_floating(car(args));

	for (args = cdr(args); is_pair(args); args = cdr(args))
		acc -= num_to_floating(car(args));
	return of_floating(acc);
}

obj sub_pp(obj args)
{
	obj chk = chknums("sub", args);
	bool flts = chknums_flt(chk) > -1;

	if (is_err(chk))
		return chk;

	switch (chknums_len(chk)) {
	case 0:
		eprintf(AREA, "'-' given no arguments");
		return error_argument_type();
	case 1:
		return flts ? of_floating(-to_floating(car(args))) :
			      of_integer(-to_integer(car(args)));
	default:
		return flts ? sub_pp_floating(args) : sub_pp_integer(args);
	}
}

obj mul_pp(obj args)
{
	INTEGER acc = 1;
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
	INTEGER acc;
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
