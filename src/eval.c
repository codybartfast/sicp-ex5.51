#include <stdbool.h>
#include "eval.h"
#include "error.h"

#define AREA "EVAL"

static bool isself_evaluating(obj expr);

obj eval(obj expr)
{
	if (isself_evaluating(expr))
		return expr;
	eprintf(AREA, "Can't parse expr:... err, can't yet display it either");
	return error_eval();
}

static bool isself_evaluating(obj expr)
{
	return Obj.isnumber(expr);
}
