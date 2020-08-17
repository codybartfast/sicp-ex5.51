#include "eval.h"

#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "list.h"
#include "output.h"

#define AREA "EVAL"

static obj apply(obj procedure, obj arguments);
static obj list_of_values(obj exps, obj env);
static obj eval_definition(obj exp, obj env);
static bool is_self_evaluating(obj exp);
static bool is_variable(obj exp);
static bool is_definition(obj exp);
static obj definition_variable(obj exp);
static obj definition_value(obj exp);
static obj make_lambda(obj, obj);
static bool is_application(obj exp);
static obj operator(obj exp);
static obj operands(obj exp);
static bool no_operands(obj ops);
static obj first_operand(obj ops);
static obj rest_operands(obj ops);
static obj apply_primitive_procedure(obj proc, obj args);

obj eval(obj exp, obj env)
{
	if (is_self_evaluating(exp))
		return exp;
	if (is_variable(exp))
		return lookup_variable_value(exp, env);
	if (is_definition(exp))
		return eval_definition(exp, env);
	if (is_application(exp)) {
		obj oator, orands;
		oator = eval(operator(exp), env);
		if (is_err(oator))
			return oator;
		orands = list_of_values(operands(exp), env);
		if (is_err(orands))
			return orands;
		return apply(oator, orands);
	}
	return error_eval(AREA, "Unknown expression type: %s", errstr(exp));
}

static obj apply(obj procedure, obj arguments)
{
	if (is_primproc(procedure)) {
		return apply_primitive_procedure(procedure, arguments);
	} else {
		return error_eval(AREA, "Unknown procedure type: %s",
				  errstr(procedure));
	}
}

// ln 72
static obj list_of_values(obj exps, obj env)
{
	return no_operands(exps) ?
		       emptylst :
		       cons(eval(first_operand(exps), env),
			    list_of_values(rest_operands(exps), env));
}

// ln 100
static obj eval_definition(obj exp, obj env)
{
	obj r = define_variable(definition_variable(exp),
				eval(definition_value(exp), env), env);
	return is_err(r) ? r : ok;
}

// ln 110
static bool is_self_evaluating(obj exp)
{
	return is_number(exp);
}

// ln 115
static bool is_variable(obj exp)
{
	return is_symbol(exp);
}

// until we have a proper eq? - also in environment
static bool eq_symbol(obj a, obj b)
{
	return is_symbol(a) && is_symbol(b) &&
	       strcmp(to_string(a), to_string(b)) == 0;
}

// ln 122
static bool is_tagged_list(obj exp, obj tag)
{
	return is_pair(exp) ? eq_symbol(car(exp), tag) : false;
}

// ln 132
static bool is_definition(obj exp)
{
	return is_tagged_list(exp, define);
}

// ln 134
static obj definition_variable(obj exp)
{
	return is_symbol(cadr(exp)) ? cadr(exp) : caadr(exp);
}

// ln 138
static obj definition_value(obj exp)
{
	return is_symbol(cadr(exp)) ?
		       caddr(exp) :
		       make_lambda(cdadr(exp), // formal parameters
				   cddr(exp)); // body
}

// ln 148
obj make_lambda(obj parameters, obj body)
{
	return cons(lambda, cons(parameters, body));
}

// ln 175
static bool is_application(obj exp)
{
	return is_pair(exp);
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
	return is_null(ops);
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

/**********************************
 *                                *
 * lns 233-317, see environment.c *
 *                                *
 **********************************/

// ln 324
static obj apply_primitive_procedure(obj proc, obj args)
{
	return (to_function(proc))(args);
}
