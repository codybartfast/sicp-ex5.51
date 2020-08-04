#include <stdbool.h>
#include "eval.h"
#include "error.h"

static bool isself_evaluating(obj *expr);

obj *eval(obj *expr)
{
	if (isself_evaluating(expr)) {
		return expr;
	}
	return error_eval();
}

static bool isself_evaluating(obj *expr)
{
	return Obj.isnumber(expr);
}
