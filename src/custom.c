#include "custom.h"
#include "environment.h"
#include "error.h"
#include "list.h"
#include "parser.h"
#include "primproc.h"

#define AREA "CUSTOM"

static obj ex_arg(obj arg)
{
	if (!is_string(arg))
		return error_argument_type(
			AREA, "%%ex expects a string of from \"1.1\"");
	return arg;
}

static obj exercise(obj args)
{
	obj r;
	if (is_err(r = chkarity("%ex", 1, args)))
		return r;
	if (is_err(r = ex_arg(args)))
		return r;

	return unspecified;
}

obj do_head(struct inport *in)
{
	obj exp = readp(in);
	if (is_pair(exp) && eq_symbol(car(exp), _ex)) {
		obj r = exercise(cdr(exp));
		if (is_err(r))
			return r;
		exp = readp(in);
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
