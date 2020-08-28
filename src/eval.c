/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/ec-evaluator-00.scm */

#include "eval.h"

#include <stdlib.h>
#include "error.h"
#include "list.h"
#include "mceval.h"
#include "output.h"
#include "primproc.h"

#define AREA "EVAL"

static obj argl;
static obj cont;
static obj expr;
static obj proc;
static obj unev;
static obj val;
static obj stack;

static void save(obj dat)
{
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

// new
static void timed_eval(obj start)
{
	obj end = runtime(emptylst);
	obj elapsed = sub(list2(end, start));
	obj secs = seconds(list1(elapsed));
	displaydat(of_string(" [ time: "));
	displaydat(secs);
	displaydat(of_string("s ] "));
}

obj eval(obj expression, obj env)
{
	expr = expression;
	stack = emptylst;
	cont = return_caller;

// 5.4.1 The Core of the Evaluator

// ln 236
eval_dispatch:
	if (is_err(expr))
		return expr;

	if (is_self_evaluating(expr))
		goto ev_self_eval;
	if (is_variable(expr))
		goto ev_variable;
	if (is_definition(expr))
		goto ev_definition;
	if (is_if(expr))
		goto ev_if;
	if (is_lambda(expr))
		goto ev_lambda;
	if (is_begin(expr))
		goto ev_begin;
	if (is_cond(expr))
		goto ev_cond;
	if (is_time(expr))
		goto ev_timed;
	if (is_application(expr))
		goto ev_application;
	goto unknown_expression_type;

// Evaluating Simple Expressions

// ln 259
ev_self_eval:
	val = expr;
	goto go_cont;
ev_variable:
	val = lookup_variable_value(expr, env);
	goto go_cont;
ev_lambda:
	unev = lambda_parameters(expr);
	expr = lambda_body(expr);
	val = make_procedure(unev, expr, env);
	goto go_cont;

// Evaluating procedure applications

// ln 277
ev_application:
	save(cont);
	save(env);
	unev = operands(expr);
	save(unev);
	expr = operator(expr);
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
	expr = first_operand(unev);
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

// ln 313
ev_appl_last_arg:
	cont = ev_appl_accum_last_arg;
	goto eval_dispatch;
ev_appl_accum_last_arg:
	argl = restore();
	argl = adjoin_arg(val, argl);
	proc = restore();
	goto apply_dispatch;

// Procedure application

// ln 324
apply_dispatch:
	if (is_err(proc))
		return proc;
	if (is_primitive_procedure(proc))
		goto primitive_apply;
	if (is_compound_procedure(proc))
		goto compound_apply;
	goto unknown_procedure_type;

// ln 331
primitive_apply:
	val = apply_primitive_procedure(proc, argl);
	if (is_err(val))
		return val;
	cont = restore();
	goto go_cont;

// ln 348
ev_begin:
	unev = begin_actions(expr);
	save(cont);
	goto ev_sequence;

// ln 338
compound_apply:
	unev = procedure_parameters(proc);
	env = procedure_environment(proc);
	env = extend_environment(unev, argl, env);
	if (is_err(env))
		return env;
	unev = procedure_body(proc);
	goto ev_sequence;

// 5.4.2 Sequence Evaluation and Tail Recursion

// ln 353
ev_sequence:
	expr = first_exp(unev);
	if (is_last_exp(unev))
		goto ev_sequence_last_exp;
	save(unev);
	save(env);
	cont = ev_sequence_continue;
	goto eval_dispatch;
ev_sequence_continue:
	env = restore();
	unev = restore();
	unev = rest_exps(unev);
	goto ev_sequence;
ev_sequence_last_exp:
	cont = restore();
	goto eval_dispatch;

// 5.4.3 Conditionals, Assignments and Definitions

// ln 374
ev_if:
	save(expr); // save expression for later
	save(env);
	save(cont);
	cont = ev_if_decide;
	expr = if_predicate(expr);
	goto eval_dispatch; // evaluate the predicate

// ln 382
ev_if_decide:
	cont = restore();
	env = restore();
	expr = restore();
	if (is_true(val))
		goto ev_if_consequent;

	// ln 389
	//ev_if_alternative:
	expr = if_alternate(expr);
	goto eval_dispatch;
ev_if_consequent:
	expr = if_consequent(expr);
	goto eval_dispatch;

// ln 416
ev_definition:
	unev = definition_variable(expr);
	save(unev); // save variable for later
	expr = definition_value(expr);
	save(env);
	save(cont);
	cont = ev_definition_1;
	goto eval_dispatch; // evaluate the definition value
ev_definition_1:
	cont = restore();
	env = restore();
	unev = restore();
	define_variable(unev, val, env);
	val = ok;
	goto go_cont;

ev_cond:
	expr = cond_to_if(expr);
	goto eval_dispatch;

// new
ev_timed:
	save(runtime(emptylst));
	save(cont);
	cont = ev_timed_done;
	expr = cons(begin, cdr(expr));
	goto eval_dispatch;

ev_timed_done:
	cont = restore();
	timed_eval(restore());
	goto go_cont;

// ln 433
unknown_expression_type:
	return error_eval(AREA, "Unknown expression type: %s", errstr(expr));
unknown_procedure_type:
	cont = restore(); // clean up stack (from apply-dispatch)
	return error_eval(AREA, "Unknown procedure type: %s", errstr(proc));

go_cont:
	if (eq_symbol(cont, return_caller))
		return val;
	if (eq_symbol(cont, ev_appl_accum_last_arg))
		goto ev_appl_accum_last_arg;
	if (eq_symbol(cont, ev_appl_accumulate_arg))
		goto ev_appl_accumulate_arg;
	if (eq_symbol(cont, ev_appl_did_operator))
		goto ev_appl_did_operator;
	if (eq_symbol(cont, ev_definition_1))
		goto ev_definition_1;
	if (eq_symbol(cont, ev_if_decide))
		goto ev_if_decide;
	if (eq_symbol(cont, ev_sequence_continue))
		goto ev_sequence_continue;
	if (eq_symbol(cont, ev_timed_done))
		goto ev_timed_done;
	return error_internal(AREA, "BUG! Goto... where?: %s", to_string(cont));
}
