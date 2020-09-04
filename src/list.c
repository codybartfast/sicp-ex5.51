#include "list.h"

#include "stdarg.h"
#include "error.h"
#include "obj.h"

#define AREA "LIST"

obj list1(obj a)
{
	return cons(a, emptylst);
}

obj list2(obj a, obj b)
{
	return cons(a, cons(b, emptylst));
}

obj list3(obj a, obj b, obj c)
{
	return cons(a, cons(b, cons(c, emptylst)));
}

obj list4(obj a, obj b, obj c, obj d)
{
	return cons(a, cons(b, cons(c, cons(d, emptylst))));
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
	if (!is_pairptr(lst)) {
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
	if (!is_pairptr(lst)) {
		eprintf(AREA, "'length' given non-pair: %s");
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
	if (!is_pairptr(lst)) {
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
	if (!is_pairptr(lst)) {
		return error_argument_type(AREA, "reverse given non-list");
	}
	return reverse_i(cdr(lst), cons(car(lst), rev));
}

obj reverse(obj lst)
{
	return reverse_i(lst, emptylst);
}

static obj append_i(obj lst1, obj lst2)
{
	if (is_null(lst1))
		return lst2;
	return append_i(cdr(lst1), cons(car(lst1), lst2));
}

obj append(obj lst1, obj lst2)
{
	lst1 = reverse(lst1);
	if (is_err(lst1))
		return lst1;
	return append_i(lst1, lst2);
}

// ACCESSORS

// TWO

obj caar(obj lst)
{
	return car(car(lst));
}

obj cadr(obj lst)
{
	return car(cdr(lst));
}
obj cdar(obj lst)
{
	return cdr(car(lst));
}

obj cddr(obj lst)
{
	return cdr(cdr(lst));
}

// THREE

obj caaar(obj lst)
{
	return car(car(car(lst)));
}

obj caadr(obj lst)
{
	return car(car(cdr(lst)));
}
obj cadar(obj lst)
{
	return car(cdr(car(lst)));
}

obj caddr(obj lst)
{
	return car(cdr(cdr(lst)));
}

obj cdaar(obj lst)
{
	return cdr(car(car(lst)));
}

obj cdadr(obj lst)
{
	return cdr(car(cdr(lst)));
}
obj cddar(obj lst)
{
	return cdr(cdr(car(lst)));
}

obj cdddr(obj lst)
{
	return cdr(cdr(cdr(lst)));
}

// FOUR

obj caaaar(obj lst)
{
	return car(car(car(car(lst))));
}

obj caaadr(obj lst)
{
	return car(car(car(cdr(lst))));
}
obj caadar(obj lst)
{
	return car(car(cdr(car(lst))));
}

obj caaddr(obj lst)
{
	return car(car(cdr(cdr(lst))));
}

obj cadaar(obj lst)
{
	return car(cdr(car(car(lst))));
}

obj cadadr(obj lst)
{
	return car(cdr(car(cdr(lst))));
}
obj caddar(obj lst)
{
	return car(cdr(cdr(car(lst))));
}

obj cadddr(obj lst)
{
	return car(cdr(cdr(cdr(lst))));
}

obj cdaaar(obj lst)
{
	return cdr(car(car(car(lst))));
}

obj cdaadr(obj lst)
{
	return cdr(car(car(cdr(lst))));
}
obj cdadar(obj lst)
{
	return cdr(car(cdr(car(lst))));
}

obj cdaddr(obj lst)
{
	return cdr(car(cdr(cdr(lst))));
}

obj cddaar(obj lst)
{
	return cdr(cdr(car(car(lst))));
}

obj cddadr(obj lst)
{
	return cdr(cdr(car(cdr(lst))));
}
obj cdddar(obj lst)
{
	return cdr(cdr(cdr(car(lst))));
}

obj cddddr(obj lst)
{
	return cdr(cdr(cdr(cdr(lst))));
}
