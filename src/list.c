#include "list.h"

#include <stdarg.h>
#include "error.h"
#include "obj.h"
#include "primproc.h"

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

obj list(obj args)
{
	return args;
}

obj is_null_p(obj args)
{
	chkarity("null?", 1, args);
	return is_null(car(args)) ? true_o : false_o;
}

obj is_pair_p(obj args)
{
	chkarity("pair?", 1, args);
	return is_pair(car(args)) ? true_o : false_o;
}

int length_i(obj lst, int len, bool prn_err)
{
	if (is_null(lst))
		return len;
	if (!is_pair(lst)) {
		if (prn_err)
			eprintf(AREA, "'length' given improper list: %s",
				errstr(lst));
		return -1;
	}
	return length_i(cdr(lst), len + 1, prn_err);
}

int length_u(obj lst)
{
	return length_i(lst, 0, true);
}

obj length(obj lst)
{
	int len = length_u(lst);
	return len < 0 ? error_argument_type(AREA, "'length' given non-list") :
			 of_integer(len);
}

obj length_p(obj args)
{
	obj chk = chkarity("length", 1, args);
	if (is_err(chk))
		return chk;
	return length(car(chk));
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

obj reverse_p(obj args)
{
	args = chkarity("reverse", 1, args);
	if (is_err(args))
		return args;
	return reverse(car(args));
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

obj append_p(obj args)
{
	obj lst = emptylst;
	for (args = reverse(args); is_pair(args); args = cdr(args)) {
		lst = append(car(args), lst);
	}
	return lst;
}

// ACCESSORS - internal

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

// ACCESSORS - primitive

// TWO

obj caar_p(obj args)
{
	args = chkarity("caar", 1, args);
	return is_err(args) ? args : car(car(car(args)));
}

obj cadr_p(obj args)
{
	args = chkarity("cadr", 1, args);
	return is_err(args) ? args : car(cdr(car(args)));
}
obj cdar_p(obj args)
{
	args = chkarity("cdar", 1, args);
	return is_err(args) ? args : cdr(car(car(args)));
}

obj cddr_p(obj args)
{
	args = chkarity("cddr", 1, args);
	return is_err(args) ? args : cdr(cdr(car(args)));
}

// THREE

obj caaar_p(obj args)
{
	args = chkarity("caaar", 1, args);
	return is_err(args) ? args : car(car(car(car(args))));
}

obj caadr_p(obj args)
{
	args = chkarity("caadr", 1, args);
	return is_err(args) ? args : car(car(cdr(car(args))));
}
obj cadar_p(obj args)
{
	args = chkarity("cadar", 1, args);
	return is_err(args) ? args : car(cdr(car(car(args))));
}

obj caddr_p(obj args)
{
	args = chkarity("caddr", 1, args);
	return is_err(args) ? args : car(cdr(cdr(car(args))));
}

obj cdaar_p(obj args)
{
	args = chkarity("cdaar", 1, args);
	return is_err(args) ? args : cdr(car(car(car(args))));
}

obj cdadr_p(obj args)
{
	args = chkarity("cdadr", 1, args);
	return is_err(args) ? args : cdr(car(cdr(car(args))));
}
obj cddar_p(obj args)
{
	args = chkarity("cddar", 1, args);
	return is_err(args) ? args : cdr(cdr(car(car(args))));
}

obj cdddr_p(obj args)
{
	args = chkarity("cdddr", 1, args);
	return is_err(args) ? args : cdr(cdr(cdr(car(args))));
}

// FOUR

obj caaaar_p(obj args)
{
	args = chkarity("caaaar", 1, args);
	return is_err(args) ? args : car(car(car(car(car(args)))));
}

obj caaadr_p(obj args)
{
	args = chkarity("caaadr", 1, args);
	return is_err(args) ? args : car(car(car(cdr(car(args)))));
}
obj caadar_p(obj args)
{
	args = chkarity("caadar", 1, args);
	return is_err(args) ? args : car(car(cdr(car(car(args)))));
}

obj caaddr_p(obj args)
{
	args = chkarity("caaddr", 1, args);
	return is_err(args) ? args : car(car(cdr(cdr(car(args)))));
}

obj cadaar_p(obj args)
{
	args = chkarity("cadaar", 1, args);
	return is_err(args) ? args : car(cdr(car(car(car(args)))));
}

obj cadadr_p(obj args)
{
	args = chkarity("cadadr", 1, args);
	return is_err(args) ? args : car(cdr(car(cdr(car(args)))));
}
obj caddar_p(obj args)
{
	args = chkarity("caddar", 1, args);
	return is_err(args) ? args : car(cdr(cdr(car(car(args)))));
}

obj cadddr_p(obj args)
{
	args = chkarity("cadddr", 1, args);
	return is_err(args) ? args : car(cdr(cdr(cdr(car(args)))));
}

obj cdaaar_p(obj args)
{
	args = chkarity("cdaaar", 1, args);
	return is_err(args) ? args : cdr(car(car(car(car(args)))));
}

obj cdaadr_p(obj args)
{
	args = chkarity("cdaadr", 1, args);
	return is_err(args) ? args : cdr(car(car(cdr(car(args)))));
}
obj cdadar_p(obj args)
{
	args = chkarity("cdadar", 1, args);
	return is_err(args) ? args : cdr(car(cdr(car(car(args)))));
}

obj cdaddr_p(obj args)
{
	args = chkarity("cdaddr", 1, args);
	return is_err(args) ? args : cdr(car(cdr(cdr(car(args)))));
}

obj cddaar_p(obj args)
{
	args = chkarity("cddaar", 1, args);
	return is_err(args) ? args : cdr(cdr(car(car(car(args)))));
}

obj cddadr_p(obj args)
{
	args = chkarity("cddadr", 1, args);
	return is_err(args) ? args : cdr(cdr(car(cdr(car(args)))));
}
obj cdddar_p(obj args)
{
	args = chkarity("cdddar", 1, args);
	return is_err(args) ? args : cdr(cdr(cdr(car(car(args)))));
}

obj cddddr_p(obj args)
{
	args = chkarity("cddddr", 1, args);
	return is_err(args) ? args : cdr(cdr(cdr(cdr(car(args)))));
}
