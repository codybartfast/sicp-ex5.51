#include "primproc.h"

#ifdef _WIN32
#define _CRT_SILENCE_NONCONFORMING_TGMATH_H
#endif

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include "error.h"
#include "list.h"
#include "output.h"
#include "string.h"

#define AREA "PRIMPROC"

enum op { ADD, SUB, MUL, DIV, MIN, MAX };
enum un { ABS, EXP, LOG, INC, DEC, SIN, COS, FLR, SQRT };
enum cmp { LT, LTE, EQ, GTE, GT };

const Integer add_max = (((Integer)1) << ((sizeof(Integer) * 8) - 2)) - 1;
const Integer add_min = -(((Integer)1) << ((sizeof(Integer) * 8) - 2));
const Integer mul_max = ((Integer)1) << ((sizeof(Integer) * 4) - 1);
const Integer mul_min = -(((Integer)1) << ((sizeof(Integer) * 4) - 1));

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
	if (act != expct)
		return error_arity(AREA,
				   "'%s' expects %d args but was given %d: %s",
				   fname, expct, act, errstr(args));
	return args;
}

// obj chkarityrng(char *fname, obj args, int min, int max, int *act)
// {
// 	obj len = length(args);
// 	if (is_err(len))
// 		return error_argument_type(
// 			AREA, "'%s' given an improper list of arguments: %s",
// 			errstr(args));
// 	*act = to_integer(len);
// 	if (*act < min || max < *act)
// 		return error_arity(
// 			AREA,
// 			"'%s' expects between %d and %d args but was given %d: %s",
// 			fname, min, max, act, errstr(args));
// 	return args;
// }

static obj allnum(char *fname, obj args)
{
	obj lst;
	for (lst = args; is_pair(lst); lst = cdr(lst)) {
		if (!is_number(car(lst)))
			return err_notnum(fname, car(lst));
	}
	return args;
}

static obj cnv_to_fltnum(const obj n)
{
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		return n;
	case NUMBER_INTEGER:
		return of_floating((Floating)to_integer(n));
	default:
		return error_internal(
			AREA, "BUG! No cnv_to_fltnum case for: %d", subtype(n));
	}
}

static obj cnv_to_intnum(const char *fname, const obj n)
{
	Floating f;
	Integer i;
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		f = to_floating(n);
		i = (Integer)f;
		return (i == f) ?
			       of_integer(i) :
			       error_argument_value(AREA,
						    "%s got a non-integer: %Lg",
						    fname, (long double)f);
	case NUMBER_INTEGER:
		return n;
	default:
		return error_internal(
			AREA, "BUG! No cnv_to_intnum case for: %d", subtype(n));
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
		       of_floating((Floating)i) :
		       n;
}

static obj chkszmul(obj n)
{
	Integer i;
	return (subtype(n) == NUMBER_INTEGER &&
		((i = to_integer(n)) < mul_min || mul_max < i)) ?
		       of_floating((Floating)i) :
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
		case MIN:
			return (a < b) ? arg1 : arg2;
		case MAX:
			return (a > b) ? arg1 : arg2;
		}
	} else {
		Floating a, b;
		obj ao = cnv_to_fltnum(arg1);
		obj bo = cnv_to_fltnum(arg2);

		if (is_err(ao))
			return ao;
		if (is_err(bo))
			return bo;
		a = to_floating(ao);
		b = to_floating(bo);

		switch (op) {
		case ADD:
			return of_floating(a + b);
		case SUB:
			return of_floating(a - b);
		case MUL:
			return of_floating(a * b);
		case DIV:
			return divf(a, b);
		case MIN:
			return (a < b) ? arg1 : arg2;
		case MAX:
			return (a > b) ? arg1 : arg2;
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

obj add(const obj args)
{
	return reduce("+ (add)", ADD, zero, args);
}

obj sub(const obj args)
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

obj mul(const obj args)
{
	return reduce("* (mul)", MUL, one, args);
}

obj divd(const obj args)
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

obj minimum(const obj args)
{
	if (!is_pair(args))
		error_arity(AREA, "'min' expects at least 1 argument");
	return reduce("min", MIN, car(args), cdr(args));
}

obj maximum(const obj args)
{
	if (!is_pair(args))
		error_arity(AREA, "'max' expects at least 1 argument");
	return reduce("max", MAX, car(args), cdr(args));
}

obj rem(const obj args)
{
	obj err, a, b;
	char *fname = "remainder";

	if (is_err(err = chkarity(fname, 2, args)))
		return err;
	if (is_err(err = allnum(fname, args)))
		return err;
	a = cnv_to_intnum(fname, car(args));
	b = cnv_to_intnum(fname, cadr(args));
	if (is_err(a))
		return a;
	if (is_err(b))
		return b;
	return of_integer(to_integer(a) % to_integer(b));
}

static obj applyun(char *fname, enum un op, obj args)
{
	obj err, n;

	if (is_err(err = chkarity(fname, 1, args)))
		return err;
	if (is_err(err = allnum(fname, args)))
		return err;
	n = car(args);
	switch (op) {
	case ABS:
		return (subtype(n) == NUMBER_INTEGER) ?
			       of_integer(llabs(to_integer(n))) :
			       of_floating(fabsl(to_floating(n)));
	case EXP:
		return of_floating(exp(to_floating(cnv_to_fltnum(n))));
	case LOG:
		return of_floating(log(to_floating(cnv_to_fltnum(n))));
	case INC:
		return applyop(ADD, n, one);
	case DEC:
		return applyop(SUB, n, one);
	case SIN:
		return of_floating(sin(to_floating(cnv_to_fltnum(n))));
	case COS:
		return of_floating(cos(to_floating(cnv_to_fltnum(n))));
	case FLR:
		return of_floating(floor(to_floating(cnv_to_fltnum(n))));
	case SQRT:
		return of_floating(sqrt(to_floating(cnv_to_fltnum(n))));
	default:
		return error_internal(AREA, "BUG! no unary case for %d", op);
	}
}

obj absl(const obj args)
{
	return applyun("abs", ABS, args);
}

obj expn(const obj args)
{
	return applyun("exp", EXP, args);
}

obj logn(const obj args)
{
	return applyun("log", LOG, args);
}

obj inc(const obj args)
{
	return applyun("inc", INC, args);
}

obj dec(const obj args)
{
	return applyun("dec", DEC, args);
}

obj sine(const obj args)
{
	return applyun("sin", SIN, args);
}

obj cosine(const obj args)
{
	return applyun("cos", COS, args);
}

obj flr(const obj args)
{
	return applyun("floor", FLR, args);
}

obj sqroot(const obj args)
{
	return applyun("sqrt", SQRT, args);
}

#ifdef _WIN32
errno_t rand_s(unsigned *);
#define PLAT_RAND_MAX UINT_MAX
#else
#define PLAT_RAND_MAX RAND_MAX
#endif

static Integer plat_rand(void)
{
#ifdef _WIN32
	unsigned r;
	return rand_s(&r);
#else
	return rand();
#endif
}

obj rnd(obj args)
{
	static bool seeded = false;
	obj nobj;
	Integer n, limit, r;

	if (!seeded) {
		srand((unsigned)time(NULL));
		seeded = true;
	}
	if (is_err(args = chkarity("random", 1, args)))
		return args;
	if (is_err(args = allnum("random", args)))
		return args;
	nobj = car(args);
	if (subtype(nobj) != NUMBER_INTEGER)
		return error_argument_type(
			AREA, "random expects an integer but got %s",
			errstr(nobj));
	n = to_integer(nobj);
	if (n <= 0 || PLAT_RAND_MAX < n)
		return error_argument_value(
			AREA, "random expects a value from 1 to %d, got %s",
			PLAT_RAND_MAX, errstr(nobj));
	limit = PLAT_RAND_MAX - (PLAT_RAND_MAX % n);
	while ((r = plat_rand()) >= limit)
		;
	return of_integer(r % n);
}

obj runtime(obj args)
{
	char *fname = "runtime";

	if (is_err(args = chkarity(fname, 0, args)))
		return args;
	if (is_err(args = allnum(fname, args)))
		return args;

	return of_integer(clock());
}

obj ticks(obj args)
{
	if (is_err(args = chkarity("clicks", 0, args)))
		return args;
	return of_integer(CLOCKS_PER_SEC);
}

obj seconds(obj args)
{
	char *fname = "seconds";
	obj nobj;

	if (is_err(args = chkarity(fname, 1, args)))
		return args;
	if (is_err(args = allnum(fname, args)))
		return args;
	nobj = car(args);
	if (subtype(nobj) != NUMBER_INTEGER)
		return error_argument_value(AREA, "%s expects an integer: %s",
					    fname, errstr(nobj));

	return of_floating(((Floating)to_integer(nobj)) /
			   ((Floating)CLOCKS_PER_SEC));
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
		Floating a, b;
		obj ao = cnv_to_fltnum(arg1);
		obj bo = cnv_to_fltnum(arg2);

		if (is_err(ao))
			return ao;
		if (is_err(bo))
			return bo;
		a = to_floating(ao);
		b = to_floating(bo);

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

obj lt(const obj args)
{
	return chkfold("< (less than)", LT, args);
}

obj lte(const obj args)
{
	return chkfold("<= (less than or equal)", LTE, args);
}

obj eqn(const obj args)
{
	return chkfold("= (equal)", EQ, args);
}

obj gte(const obj args)
{
	return chkfold(">= (greater than)", GTE, args);
}

obj gt(const obj args)
{
	return chkfold("> (greater than)", GT, args);
}

obj not(const obj args)
{
	obj chk;
	if (is_err(chk = chkarity("not", 1, args)))
		return chk;
	return is_false(car(args)) ? tru_o : fls_o;
}

obj consp(const obj args)
{
	obj chk;
	if (is_err(chk = chkarity("cons", 2, args)))
		return chk;
	return cons(car(args), cadr(args));
}

obj carp(const obj args)
{
	obj chk;
	if (is_err(chk = chkarity("car", 1, args)))
		return chk;
	return caar(args);
}

obj cdrp(const obj args)
{
	obj chk;
	if (is_err(chk = chkarity("cdr", 1, args)))
		return chk;
	return cdar(args);
}

// EQUALITY

bool is_eq(obj a, obj b)
{
	if (type(a) != type(b)) {
		return false;
	}
	switch (type(a)) {
	case TYPE_SYMBOL:
		return strcmp(to_string(a), to_string(b)) == 0;
	case TYPE_PAIRPTR:
		return to_pairptr(a) == to_pairptr(b);
	case TYPE_EMPTY_LIST:
		return true;
	case TYPE_NUMBER:
		return is_true(applycmp(EQ, a, b));
	default:
		return false;
	}
}

obj is_eq_p(const obj args)
{
	obj chk;

	if (is_err(chk = chkarity("eq?", 2, args)))
		return chk;
	return is_eq(car(args), cadr(args)) ? tru_o : fls_o;
}

static bool is_equal(obj a, obj b)
{
	if (is_eq(a, b))
		return true;
	if (is_pair(a) && is_pair(b) && is_equal(car(a), car(b)))
		return is_equal(cdr(a), cdr(b));
	return false;
}

obj is_equal_p(const obj args)
{
	obj chk;

	if (is_err(chk = chkarity("eq?", 2, args)))
		return chk;
	return is_equal(car(args), cadr(args)) ? tru_o : fls_o;
}
