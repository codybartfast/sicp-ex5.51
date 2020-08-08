#include "error.h"
#include "pair.h"

#define AREA "PAIR"

// PAIR

inline bool ispair(obj dat)
{
	return dat.ispair;
}

inline bool isnull(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_EMPTY_LIST;
}

const obj empty_inst = {
	false, .simp = { TYPE_EMPTY_LIST, SUBTYPE_NOT_SET, { 0 } }
};

inline static obj empty(void)
{
	return empty_inst;
}

obj cons(obj car, obj cdr)
{
	obj p = { true,
		  .pair = { ispair(car) ? Obj.reference(car).simp : car.simp,
			    ispair(cdr) ? Obj.reference(cdr).simp : cdr.simp } };
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

obj set_car(obj pair);
obj set_cdr(obj pair);
