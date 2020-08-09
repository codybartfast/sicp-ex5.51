#include "error.h"
#include "obj.h"
#include "list.h"

#define AREA "LIST"

static obj reversei(obj src, obj dst);
obj reverse(obj lst)
{
	return reversei(lst, Obj.empty());
}

static obj reversei(obj src, obj dst)
{
	if (isnull(src))
		return dst;
	if (!ispair(src)) {
		eprintf(AREA, "reverse got non list");
		error_argument_type();
	}
	return reversei(cdr(src), cons(car(src), dst));
}
