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
		//define_variable(of_identifier("abs"), of_function(abs_pp), env);
		evalstr("(define (abs x) (if (< x 0) (- x) x))", env);

		//define_variable(of_identifier("<="), of_function(lte_pp), env);
		evalstr("(define (<= x y) (not (> x y)))", env);

		//define_variable(of_identifier(">="), of_function(gte_pp), env);
		evalstr("(define (>= x y) (not (< x y)))", env);

		evalstr("(define (square x) (* x x))", env);
	}
	if (ex >= 109) {
		define_variable(of_identifier("exp"), of_function(exp_pp), env);
		define_variable(of_identifier("log"), of_function(log_pp), env);
	}
	if (ex >= 115) {
		define_variable(of_identifier("remainder"), of_function(rem_pp),
				env);
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
		newline();
		displayp(out, lpad);
		displayp(out, car(names));
	}
	return emptystr;
}

obj display_defined(obj _)
{
	(void)_;
	return display_definedp(default_out());
}
