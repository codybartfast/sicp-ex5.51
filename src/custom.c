#include "custom.h"

#include <ctype.h>
#include <stdlib.h>
#include "environment.h"
#include "error.h"
#include "eval.h"
#include "list.h"
#include "parser.h"
#include "primproc.h"

#define AREA "CUSTOM"

static obj evalstr(char *e, obj env)
{
	return eval(readp(openin_string(e)), env);
}

static obj add_extras(int ex, obj env)
{
	if (ex > 101) {
		//define_variable(of_identifier("abs"), of_function(absl), env);
		evalstr("(define (abs x) (if (< x 0) (- x) x))", env);

		//define_variable(of_identifier("<="), of_function(lte), env);
		evalstr("(define (<= x y) (not (> x y)))", env);

		//define_variable(of_identifier(">="), of_function(gte), env);
		evalstr("(define (>= x y) (not (< x y)))", env);

		evalstr("(define (square x) (* x x))", env);
		evalstr("(define (cube x) (* x x x))", env);
	}
	if (ex >= 109) {
		define_variable(of_identifier("exp"), of_function(expn), env);
		define_variable(of_identifier("log"), of_function(logn), env);
		// inc and dec could be 'defined' but making them primitive
		// avoids the risk of loops in questions where + is defined in
		// terms of inc
		define_variable(of_identifier("inc"), of_function(inc), env);
		define_variable(of_identifier("dec"), of_function(dec), env);
	}
	if (ex >= 115) {
		define_variable(of_identifier("remainder"), of_function(rem),
				env);
	}
	if (ex >= 116) {
		evalstr("(define (even? n) (= (remainder n 2) 0))", env);
		evalstr("(define (odd? n) (= (remainder n 2) 1))", env);
	}
	if (ex >= 120) {
		evalstr("(define (gcd a b) (if (= b 0) a (gcd b (rema Finder a b))))",
			env);
		define_variable(of_identifier("random"), of_function(rnd), env);
	}
	if (ex >= 121) {
		evalstr("(define (prime? n)"
			"  (define (smallest-divisor n)"
			"    (define (find-divisor n test-divisor)"
			"      (define (divides? a b)"
			"        (= (remainder b a) 0))"
			"      (cond ((> (square test-divisor) n) n)"
			"            ((divides? test-divisor n) test-divisor)"
			"            (else (find-divisor n (+ test-divisor 1)))))"
			"    (find-divisor n 2))"
			"  (= n (smallest-divisor n)))",
			env);
	}
	if (ex >= 122) {
		define_variable(of_identifier("display"), of_function(display),
				env);
		define_variable(of_identifier("ignore"), of_function(ignore),
				env);
		define_variable(of_identifier("newline"), of_function(newline),
				env);
		define_variable(of_identifier("runtime"), of_function(runtime),
				env);
		define_variable(of_identifier("seconds"), of_function(seconds),
				env);
		define_variable(of_identifier("ticks"), of_function(ticks),
				env);
	}
	if (ex >= 129) {
		evalstr("(define (identity x) x)", env);
	}
	return unspecified;
}

static int ex_num(obj arg)
{
	const char *str;
	int ok = false;
	if (is_string(arg)) {
		str = to_string(arg);
		if (isdigit(str[0]) && (str[1] == '.') && isdigit(str[2]) &&
		    (str[3] == '\0' || (isdigit(str[3]) && str[4] == '\0'))) {
			ok = true;
		}
	}
	if (!ok) {
		return -1;
	}
	return (100 * (str[0] - 48)) + atoi(str + 2);
}

static obj conf_ex(obj env, obj args)
{
	obj arg;
	int ex;
	if (is_err(arg = chkarity("%ex", 1, args)))
		return arg;
	if ((ex = ex_num(car(args))) == -1)
		return error_argument_type(
			AREA, "%%ex expects a string of from \"1.23\"");
	return add_extras(ex, env);
}

obj do_head(obj env, struct inport *in)
{
	obj exp = readp(in);
	if (is_pair(exp) && eq_symbol(car(exp), _ex)) {
		obj r = conf_ex(env, cdr(exp));
		if (is_err(r))
			return r;
		exp = readp(in);
	} else {
		add_extras(552, env);
	}
	return exp;
}

static obj defined(void)
{
	return caar(the_global_environment());
}

static obj display_definedp(struct outport *out)
{
	obj names;
	obj lpad = of_string("  ");
	displayp(out, of_string("Defined Variables:"));
	for (names = reverse(defined()); is_pair(names); names = cdr(names)) {
		newline(emptylst);
		displayp(out, lpad);
		displayp(out, car(names));
	}
	newline(emptylst);
	displayp(out, of_string("Custom Special Forms:"));
	newline(emptylst);
	displayp(out, lpad);
	displayp(out, of_string("time"));
	return _void;
}

obj display_defined(obj _)
{
	(void)_;
	return display_definedp(default_out());
}
