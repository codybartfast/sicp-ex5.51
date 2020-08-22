#include "custom.h"
#include "environment.h"
#include "error.h"
#include "list.h"

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