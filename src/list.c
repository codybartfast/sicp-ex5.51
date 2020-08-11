#include "error.h"
#include "obj.h"
#include "list.h"

#define AREA "LIST"

inline obj cadr(obj lst)
{
	return car(cdr(lst));
}

obj list2(obj a, obj b)
{
	return cons(a, cons(b, emptylst));
}

static int length_i(obj lst, int len)
{
	if (isnull(lst))
		return len;
	if (!ispair(lst)) {
		eprintf(AREA, "Length given an improper list");
		return -1;
	}
	return length_i(cdr(lst), len + 1);
}

int length_u(obj lst)
{
	if (isnull(lst)) {
		return 0;
	}
	if (!ispair(lst)) {
		eprintf(AREA, "Length given non-pair: %s");
		return -1;
	}
	return length_i(lst, 0);
}

obj length(obj lst)
{
	int len = length_u(lst);
	return len < 0 ? error_argument_type() : Obj.ofint64(len);
}

static obj map_u_i(obj (*func)(obj), obj lst, obj prj)
{
	if (isnull(lst))
		return prj;
	if (!ispair(lst)) {
		eprintf(AREA, "map_u given non-list");
		return error_argument_type();
	}
	return map_u_i(func, cdr(lst), cons(func(car(lst)), prj));
}

obj map_u(obj (*func)(obj), obj lst)
{
	return reverse(map_u_i(func, lst, emptylst));
}

static obj reverse_i(obj lst, obj rev)
{
	if (isnull(lst))
		return rev;
	if (!ispair(lst)) {
		eprintf(AREA, "reverse given non-list");
		return error_argument_type();
	}
	return reverse_i(cdr(lst), cons(car(lst), rev));
}

obj reverse(obj lst)
{
	return reverse_i(lst, emptylst);
}
