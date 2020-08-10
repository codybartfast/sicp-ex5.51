#include "error.h"
#include "output.h"
#include "list.h"

#include "primproc.h"

#define AREA "PRIMPROC"

obj add_pp(obj args)
{
	int64_t acc = 0;
	do {
		obj fst = car(args);
		if (!Obj.isnumber(fst)) {
			eprintf(AREA, "+ given non-number: %s",
				Obj.tostring(writestr(fst)));
			return error_argument_type();
		}
		acc += Obj.toint64(fst);
	} while (ispair(args = cdr(args)));
	return Obj.ofint64(acc);
}

obj sub_pp(obj args)
{
	int64_t acc;
	obj fst;
	if (!ispair(args)) {
		eprintf(AREA, "'-' given no arguments");
		return error_argument_type();
	}
	if (!Obj.isnumber(fst)) {
		eprintf(AREA, "'-' given non-number: %s",
			Obj.tostring(writestr(fst)));
		return error_argument_type();
	}
	acc = Obj.toint64(fst);
	acc = isnull(cdr(args)) ? -acc : acc;
	while (ispair(args = cdr(args))) {
		fst = car(args);
		if (!Obj.isnumber(fst)) {
			eprintf(AREA, "'-' given non-number: %s",
				Obj.tostring(writestr(fst)));
			return error_argument_type();
		}
		acc -= Obj.toint64(fst);
	}
	return Obj.ofint64(acc);
}
