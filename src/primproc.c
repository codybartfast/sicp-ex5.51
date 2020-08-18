#include "primproc.h"

#include "stdbool.h"
#include "error.h"
#include "output.h"
#include "list.h"

#define AREA "PRIMPROC"

enum op { ADD, SUB, MUL, DIV };
enum cmp { LT, LTE, EQ, GT, GTE };

static obj err_improper(const char *fname, const obj np)
{
	return error_argument_value(AREA, "%s given an improper list: %s",
				    fname, errstr(np));
}

static obj err_notnum(const char *fname, const obj np)
{
	return error_argument_value(AREA, "%s given an non-number: %s", fname,
				    errstr(np));
}

static FLOATING num_to_floating(const obj n)
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

static obj divf(const FLOATING a, const FLOATING b)
{
	if (b == 0) {
		return error_argument_value(
			AREA, "divide by zero (/ " FLOATING_FORMAT " 0)", a);
	}
	return of_floating(a / b);
}

static obj applyop(const enum op op, const obj arg1, const obj arg2)
{
	if (subtype(arg1) == NUMBER_INTEGER &&
	    subtype(arg2) == NUMBER_INTEGER) {
		const INTEGER a = to_integer(arg1), b = to_integer(arg2);
		switch (op) {
		case ADD:
			return of_integer(a + b);
		case SUB:
			return of_integer(a - b);
		case MUL:
			return of_integer(a * b);
		case DIV:
			if (b != 0 && a % b == 0)
				return of_integer(a / b);
			return divf(a, b);
		}
	} else {
		const FLOATING a = num_to_floating(arg1),
			       b = num_to_floating(arg2);
		switch (op) {
		case ADD:
			return of_floating(a + b);
		case SUB:
			return of_floating(a - b);
		case MUL:
			return of_floating(a * b);
		case DIV:
			return divf(a, b);
		}
	}
	return error_internal(AREA, "BUG! no apply op case for %d", op);
}

static obj checkcar(const char *fname, const obj args)
{
	if (is_null(args)) {
		return error_argument_value(
			AREA, "%s requires at least one argument", fname);
	}
	if (!is_pair(args)) {
		return err_improper(fname, args);
	} else {
		const obj fst = car(args);
		if (!is_number(fst))
			return err_notnum(fname, fst);
		return args;
	}
}

static obj reduce(const char *fname, const enum op op, const obj acc,
		  const obj args)
{
	if (is_null(args))
		return acc;
	if (!is_pair(args))
		return err_improper(fname, args);
	else {
		const obj num = car(args);
		if (!is_number(num))
			return err_notnum(fname, num);
		return reduce(fname, op, applyop(op, acc, num), cdr(args));
	}
}

obj add_pp(const obj args)
{
	return reduce("+ (add)", ADD, zero, args);
}

obj sub_pp(const obj args)
{
	const char *fname = "- (sub)";
	obj err;
	if (is_err(err = checkcar(fname, args)))
		return err;
	else {
		const obj fst = car(args), rst = cdr(args);
		if (is_null(rst)) {
			return applyop(SUB, zero, fst);
		} else {
			return reduce(fname, SUB, fst, rst);
		}
	}
}

obj mul_pp(const obj args)
{
	return reduce("* (mul)", MUL, one, args);
}

obj div_pp(const obj args)
{
	const char *fname = "/ (div)";
	obj err;
	if (is_err(err = checkcar(fname, args)))
		return err;
	else {
		const obj fst = car(args), rst = cdr(args);
		if (is_null(rst)) {
			return applyop(DIV, one, fst);
		} else {
			return reduce(fname, DIV, fst, rst);
		}
	}
}

static obj applycmp(const enum cmp cmp, const obj arg1, const obj arg2)
{
	if (subtype(arg1) == NUMBER_INTEGER &&
	    subtype(arg2) == NUMBER_INTEGER) {
		const INTEGER a = to_integer(arg1), b = to_integer(arg2);
		switch (cmp) {
		case LT:
			return a < b ? tru_o : fls_o;
		case LTE:
			return a <= b ? tru_o : fls_o;
		case EQ:
			return a == b ? tru_o : fls_o;
		case GT:
			return a >= b ? tru_o : fls_o;
		case GTE:
			return a > b ? tru_o : fls_o;
		}
	} else {
		const FLOATING a = num_to_floating(arg1),
			       b = num_to_floating(arg2);
		switch (cmp) {
		case LT:
			return a < b ? tru_o : fls_o;
		case LTE:
			return a <= b ? tru_o : fls_o;
		case EQ:
			return a == b ? tru_o : fls_o;
		case GT:
			return a >= b ? tru_o : fls_o;
		case GTE:
			return a > b ? tru_o : fls_o;
		}
	}
	return error_internal(AREA, "BUG! no comparison case for %d", cmp);
}

static obj fold(const char *fname, const enum cmp cmp, const obj prv,
		const obj args)
{
	if (is_null(args))
		return tru_o;
	if (!is_pair(args))
		return err_improper(fname, args);
	else {
		const obj num = car(args);
		if (!is_number(num))
			return err_notnum(fname, num);
		if (is_true(applycmp(cmp, prv, num))) {
			return fold(fname, cmp, num, cdr(args));
		} else {
			return fls_o;
		}
	}
}

static obj chkfold(char *fname, enum cmp cmp, obj args)
{
	obj err;
	if (is_err(err = checkcar(fname, args)))
		return err;
	return fold(fname, cmp, car(args), cdr(args));
}

obj lt_pp(const obj args)
{
	return chkfold("< (less than)", LT, args);
}

obj lte_pp(const obj args)
{
	return chkfold("<= (less than or equal)", LTE, args);
}

obj eqn_pp(const obj args)
{
	return chkfold("= (equal)", EQ, args);
}

obj gt_pp(const obj args)
{
	return chkfold("> (greater than)", GT, args);
}

obj gte_pp(const obj args)
{
	return chkfold(">= (greater than)", GTE, args);
}