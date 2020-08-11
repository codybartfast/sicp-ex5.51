#include <stdbool.h>
#include "eval.h"
#include "error.h"
#include "output.h"

#define AREA "EVAL"

static obj apply(obj procedure, obj arguments);
static obj list_of_values(obj exps, obj env);
static bool isself_evaluating(obj exp);
static bool isvariable(obj exp);
static bool isapplication(obj exp);
static obj operator(obj exp);
static obj operands(obj exp);
static bool no_operands(obj ops);
static obj first_operand(obj ops);
static obj rest_operands(obj ops);
static obj apply_primitive_procedure(obj proc, obj args);

obj eval(obj exp, obj env)
{
	if (isself_evaluating(exp))
		return exp;
	if (isvariable(exp))
		return lookup_variable_value(exp, env);
	else if (isapplication(exp)) {
		obj oator, orands;
		oator = eval(operator(exp), env);
		if (iserr(oator))
			return oator;
		orands = list_of_values(operands(exp), env);
		if (iserr(orands))
			return orands;
		// printf("Got operator: %s\n", debugstr(oator));
		// printf("Got operands: %s\n", debugstr(orands));
		return apply(oator, orands);
	}
	eprintf(AREA, "Can't parse exp: %s", Obj.tostring(writestr(exp)));
	return error_eval();
}

static obj apply(obj procedure, obj arguments)
{
	// printf("Apply proc: %s\n", Obj.tostring(writestr(procedure)));
	if (Obj.isprimproc(procedure)) {
		return apply_primitive_procedure(procedure, arguments);
	} else {
		eprintf(AREA, "Unknown procedure type: %s",
			"blah"); //writestr(procedure));
		return error_eval();
	}
}

// ln 72
static obj list_of_values(obj exps, obj env)
{
	return no_operands(exps) ?
		       Obj.empty() :
		       cons(eval(first_operand(exps), env),
			    list_of_values(rest_operands(exps), env));
}

// ln 110
static bool isself_evaluating(obj exp)
{
	return Obj.isnumber(exp);
}

//ln 115
static bool isvariable(obj exp)
{
	return Obj.issymbol(exp);
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

// ln 178
static bool no_operands(obj ops)
{
	return isnull(ops);
}

// ln 179
static obj first_operand(obj ops)
{
	return car(ops);
}

// ln 180
static obj rest_operands(obj ops)
{
	return cdr(ops);
}

//
// ln 233 - environment.c
//

// ln 324
static obj apply_primitive_procedure(obj proc, obj args)
{
	return Obj.tofunction(proc)(args);
}
