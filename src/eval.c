/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/ec-evaluator-00.scm */

#include "eval.h"

#include "environment.h"
#include "error.h"
#include "list.h"
#include "mceval.h"

#define AREA "STACK"

static obj argl;
static obj cont;
static obj exp;
static obj proc;
static obj unev;
static obj val;

#include <stdlib.h>

static obj stack = { TYPE_EMPTY_LIST, SUBTYPE_NOT_SET, { 0 } };
static void save(obj dat)
{
	// if not init then init stack and regs. then finish working
	// on go_cont...

	stack = cons(dat, stack);
	if (is_err(stack)) {
		eprintf(AREA, "save / cons memory error");
		exit(1);
	}
	return;
}

static obj restore(void)
{
	if (is_null(stack)) {
		eprintf(AREA, "attempted to restore from empty stack");
		exit(1);
	} else {
		obj dat = car(stack);
		stack = cdr(stack);
		return dat;
	}
}

// ln 182
static obj empty_arglist(void)
{
	return emptylst;
}

// ln 185
static bool is_last_operand(obj exp)
{
	return is_null(cdr(exp));
}

// ln 186 (but this time in book order)
static obj adjoin_arg(obj arg, obj arglist)
{
	return append(arglist, list1(arg));
}

obj eval(obj expression, obj env)
{
	exp = expression;

go_con:
	if(eq_symbol(cont, ev_appl_accumulate_arg))
		goto ev_appl_accumulate_arg;
	if(eq_symbol(cont, ev_appl_did_operator))
		goto ev_appl_did_operator;
	eprintf(AREA, "Goto... where?: %s", to_string(cont));
	
// 5.4.1 The Core of the Evaluator
// ===============================

// ln 236
eval_dispatch:
	if (is_err(exp))
		return exp;

	if (is_self_evaluating(exp))
		goto ev_self_eval;
	if (is_variable(exp))
		goto ev_variable;
	if (is_definition(exp))
		goto ev_definition;
	if (is_if(exp))
		goto ev_if;
	if (is_lambda(exp))
		goto ev_lambda;
	// if (is_cond(exp))
	// 	return eval(cond_to_if(exp), env);
	if (is_application(exp))
		goto ev_application;
	goto unknown_expression_type;

// Evaluating Simple Expressions
// ln 259
ev_self_eval:
	val = exp;
	goto go_con;
ev_variable:
	val = lookup_variable_value(exp, env);
	goto go_con;
ev_lambda:
	unev = lambda_parameters(exp);
	exp = lambda_body(exp);
	val = make_procedure(unev, exp, env);
	goto go_con;

// Evaluating procedure applications

// ln 277
ev_application:
	save(cont);
	save(env);
	unev = operands(exp);
	save(unev);
	exp = operator(exp);
	cont = ev_appl_did_operator;
	goto eval_dispatch;

// ln 286
ev_appl_did_operator:
	unev = restore(); // the operands
	env = restore();
	argl = empty_arglist();
	proc = val; // the operator
	if (no_operands(unev))
		goto apply_dispatch;
	save(proc);

// ln 295
ev_appl_operand_loop:
	save(argl);
	exp = first_operand(unev);
	if (is_last_operand(unev))
		goto ev_appl_last_arg;
	save(env);
	save(unev);
	cont = ev_appl_accumulate_arg;
	goto eval_dispatch;

// ln 305
ev_appl_accumulate_arg:
	unev = restore();
	env = restore();
	argl = restore();
	argl = adjoin_arg(val, argl);
	unev = rest_operands(unev);
	goto ev_appl_operand_loop;



}