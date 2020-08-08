#include "error.h"
#include "pair.h"

#define AREA "PAIR"

inline bool isnull(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_EMPTY_LIST;
}

obj cons(obj car, obj cdr)
{
	obj p = { true,
		  .pair = { ispair(car) ? Obj.reference(car).simp : car.simp,
			    ispair(cdr) ? Obj.reference(cdr).simp :
					  cdr.simp } };
	return p;
}

obj car(obj pair)
{
	obj dat = { false, .simp = pair.pair.car };
	if (!ispair(pair)) {
		eprintf(AREA, "car expects a pair");
		return error_argument_type();
	}
	return Obj.isreference(dat) ? Obj.dereference(dat) : dat;
}

obj cdr(obj pair)
{
	obj dat = { false, .simp = pair.pair.cdr };
	if (!ispair(pair)) {
		eprintf(AREA, "cdr expects a pair");
		return error_argument_type();
	}
	return Obj.isreference(dat) ? Obj.dereference(dat) : dat;
}

obj set_car(obj *pair, obj val)
{
	if (!ispair(*pair)) {
		eprintf(AREA, "set_car expects a pair");
		return error_argument_type();
	}
	(*pair).pair.car = ispair(val) ? Obj.reference(val).simp : val.simp;
	return Obj.unspecified();
}

obj set_cdr(obj *pair, obj val){
	if (!ispair(*pair)) {
		eprintf(AREA, "set_cdr expects a pair");
		return error_argument_type();
	}
	(*pair).pair.cdr = ispair(val) ? Obj.reference(val).simp : val.simp;
	return Obj.unspecified();
}
