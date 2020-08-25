#include "primproc.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "output.h"
#include "list.h"

#define AREA "PRIMPROC"

enum op { ADD, SUB, MUL, DIV };
enum un { ABS, EXP, LOG };
enum cmp { LT, LTE, EQ, GTE, GT };

const Integer add_max = (((Integer)1) << ((sizeof(Integer) * 8) - 2)) - 1;
const Integer add_min = -(((Integer)1) << ((sizeof(Integer) * 8) - 2));
const Integer mul_max = ((Integer)1) << ((sizeof(Integer) * 4) - 1);
const Integer mul_min = -mul_max;

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

obj chkarity(char *fname, int expct, obj args)
{
	obj len = length(args);
	if (is_err(len))
		return error_argument_type(
			AREA, "'%s' given an improper list of arguments: %s",
			errstr(args));
	Integer act = to_integer(len);
	if (act == expct)
		return unspecified;
	return error_arity(AREA, "'%s' expects %d args but was given %d: %s",
			   fname, expct, act, errstr(args));
}

static Floating num_to_floating(const obj n)
{
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		return to_floating(n);
	case NUMBER_INTEGER:
		return (Floating)to_integer(n);
	default:
		eprintf(AREA, "BUG! No num_to_floating case for: %d",
			subtype(n));
		return -1;
	}
}

static obj divf(const Floating a, const Floating b)
{
	if (b == 0) {
		return error_argument_value(AREA, "divide by zero (/ %Lg 0)",
					    (long double)a);
	}
	return of_floating(a / b);
}

static obj chkszadd(obj n)
{
	Integer i;
	return (subtype(n) == NUMBER_INTEGER &&
		((i = to_integer(n)) < mul_min || mul_max < i)) ?
		       of_floating(i) :
		       n;
}

static obj chkszmul(obj n)
{
	Integer i;
	return (subtype(n) == NUMBER_INTEGER &&
		((i = to_integer(n)) < mul_min || mul_max < i)) ?
		       of_floating(i) :
		       n;
}

static obj applyop(const enum op op, obj arg1, obj arg2)
{
	switch (op) {
	case ADD:
	case SUB:
		arg1 = chkszadd(arg1);
		arg2 = chkszadd(arg2);
		break;
	case MUL:
		arg1 = chkszmul(arg1);
		arg2 = chkszmul(arg2);
		break;
	default:
		break;
	}

	if (subtype(arg1) == NUMBER_INTEGER &&
	    subtype(arg2) == NUMBER_INTEGER) {
		const Integer a = to_integer(arg1), b = to_integer(arg2);
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
			return divf((Floating)a, (Floating)b);
		}
	} else {
		const Floating a = num_to_floating(arg1),
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

static obj applyun(char *fname, enum un op, obj args)
{
	obj err, n;
	if (is_err(err = chkarity(fname, 1, args)))
		return err;
	if (!is_number(n = car(args)))
		return err_notnum(fname, n);
	switch (op) {
	case ABS:
		return (subtype(n) == NUMBER_INTEGER) ?
			       of_integer(abs(to_integer(n))) :
			       of_floating(abs(to_floating(n)));
	case EXP:
		return of_floating(exp(num_to_floating(n)));
	case LOG:
		return of_floating(log(num_to_floating(n)));
	default:
		return error_internal(AREA, "BUG! no unary case for %d", op);
	}
}

obj abs_pp(const obj args)
{
	return applyun("abs", ABS, args);
}

obj exp_pp(const obj args)
{
	return applyun("exp", EXP, args);
}

obj log_pp(const obj args)
{
	return applyun("log", LOG, args);
}

static obj applycmp(const enum cmp cmp, const obj arg1, const obj arg2)
{
	if (subtype(arg1) == NUMBER_INTEGER &&
	    subtype(arg2) == NUMBER_INTEGER) {
		const Integer a = to_integer(arg1), b = to_integer(arg2);
		switch (cmp) {
		case LT:
			return a < b ? tru_o : fls_o;
		case LTE:
			return a <= b ? tru_o : fls_o;
		case EQ:
			return a == b ? tru_o : fls_o;
		case GTE:
			return a >= b ? tru_o : fls_o;
		case GT:
			return a > b ? tru_o : fls_o;
		}
	} else {
		const Floating a = num_to_floating(arg1),
			       b = num_to_floating(arg2);
		switch (cmp) {
		case LT:
			return a < b ? tru_o : fls_o;
		case LTE:
			return a <= b ? tru_o : fls_o;
		case EQ:
			return a == b ? tru_o : fls_o;
		case GTE:
			return a >= b ? tru_o : fls_o;
		case GT:
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

obj gte_pp(const obj args)
{
	return chkfold(">= (greater than)", GTE, args);
}

obj gt_pp(const obj args)
{
	return chkfold("> (greater than)", GT, args);
}

obj and_pp(const obj args)
{
	if (is_null(args))
		return tru_o;
	if (!is_pair(args))
		return err_improper("and", args);
	return is_false(car(args)) ? fls_o : and_pp(cdr(args));
}

obj or_pp(const obj args)
{
	if (is_null(args))
		return fls_o;
	if (!is_pair(args))
		return err_improper("or", args);
	return is_true(car(args)) ? tru_o : or_pp(cdr(args));
}

obj not_pp(const obj args)
{
	obj chk;
	if (is_err(chk = chkarity("not", 1, args)))
		return chk;
	return is_false(car(args)) ? tru_o : fls_o;
}
