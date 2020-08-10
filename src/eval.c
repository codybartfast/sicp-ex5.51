#include <stdbool.h>
#include "eval.h"
#include "error.h"
#include "output.h"

#define AREA "EVAL"

static obj apply(obj procedure, obj arguments);
static obj list_of_values(obj exps, obj env);
static bool isself_evaluating(obj exp);
static bool isapplication(obj exp);
static obj operator(obj exp);
static obj operands(obj exp);

obj eval(obj exp, obj env)
{
	if (isself_evaluating(exp))
		return exp;
	else if (isapplication(exp))
		return apply(eval(operator(exp), env),
			     list_of_values(operands(exp), env));
	eprintf(AREA, "Can't parse exp: %s", Obj.tostring(writestr(exp)));
	return error_eval();
}

static obj apply(obj procedure, obj arguments)
{
	(void)procedure;
	(void)arguments;
	return Obj.ofstring("Forty Two");
}

// ln 72
static obj list_of_values(obj exps, obj env){
	(void)exps;
	(void)env;
	return Obj.empty();
}

// ln 110
static bool isself_evaluating(obj exp)
{
	return Obj.isnumber(exp);
}

// ln 175
static bool isapplication(obj exp)
{
	return ispair(exp);
}

// ln 176
static obj operator(obj exp)
{
	return car(exp);
}

// ln 177
static obj operands(obj exp)
{
	return cdr(exp);
}
