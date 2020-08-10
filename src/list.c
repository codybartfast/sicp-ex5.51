#include "error.h"
#include "obj.h"
#include "list.h"

#define AREA "LIST"

static obj reverse_i(obj lst, obj rev)
{
	if (isnull(lst))
		return rev;
	if (!ispair(lst)) {
		eprintf(AREA, "reverse got non list");
		return error_argument_type();
	}
	return reverse_i(cdr(lst), cons(car(lst), rev));
}

obj reverse(obj lst)
{
	return reverse_i(lst, Obj.empty());
}
