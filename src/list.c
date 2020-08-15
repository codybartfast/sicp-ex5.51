#include "list.h"

#include "stdarg.h"
#include "error.h"
#include "obj.h"

#define AREA "LIST"

obj cadr(obj lst)
{
	return car(cdr(lst));
}

obj caddr(obj lst)
{
	return car(cdr(cdr(lst)));
}

obj list2(obj a, obj b)
{
	return cons(a, cons(b, emptylst));
}

obj list3(obj a, obj b, obj c)
{
	return cons(a, cons(b, cons(c, emptylst)));
}

static obj listn_i(int argc, va_list els, obj lst)
{
	obj element;
	if (argc <= 0) {
		return lst;
	}
	element = va_arg(els, obj);
	return listn_i(argc - 1, els, cons(element, lst));
}

obj listn(int argc, ...)
{
	va_list els;
	va_start(els, argc);
	return reverse(listn_i(argc, els, emptylst));
}

static int length_i(obj lst, int len)
{
	if (is_null(lst))
		return len;
	if (!is_pair(lst)) {
		eprintf(AREA, "Length given an improper list");
		return -1;
	}
	return length_i(cdr(lst), len + 1);
}

int length_u(obj lst)
{
	if (is_null(lst)) {
		return 0;
	}
	if (!is_pair(lst)) {
		eprintf(AREA, "Length given non-pair: %s");
		return -1;
	}
	return length_i(lst, 0);
}

obj length(obj lst)
{
	int len = length_u(lst);
	return len < 0 ? error_argument_type(AREA, "'length' given non-list") :
			 of_integer(len);
}

static obj map_u_i(obj (*func)(obj), obj lst, obj prj)
{
	if (is_null(lst))
		return prj;
	if (!is_pair(lst)) {
		return error_argument_type(AREA, "map_u given non-list");
	}
	return map_u_i(func, cdr(lst), cons(func(car(lst)), prj));
}

obj map_u(obj (*func)(obj), obj lst)
{
	return reverse(map_u_i(func, lst, emptylst));
}

static obj reverse_i(obj lst, obj rev)
{
	if (is_null(lst))
		return rev;
	if (!is_pair(lst)) {
		return error_argument_type(AREA, "reverse given non-list");
	}
	return reverse_i(cdr(lst), cons(car(lst), rev));
}

obj reverse(obj lst)
{
	return reverse_i(lst, emptylst);
}
