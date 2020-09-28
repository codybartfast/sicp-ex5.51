/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/ec-evaluator-00.scm */

#include "eval.h"

#include <stdlib.h>
#include "error.h"
#include "list.h"
#include "mceval.h"
#include "output.h"
#include "primproc.h"
#include "storage.h"

#define AREA "EVAL"

enum reg { ARGL, CONT, ENV, EXPR, PROC, STACK, UNEV, VAL };

static obj argl; // 1
static obj cont; // 2
static obj env; // 3
static obj expr; // 4
static obj proc; // 5
static obj stack; // 6
static obj unev; // 7
static obj val; // 8
// Plus... the_global_environment // 9
const int rootlen = 9;
static obj rootlst;

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

// not in book
static void timed_eval(obj start)
{
	obj end = runtime(emptylst);
	obj elapsed = sub(list2(end, start));
	obj secs = seconds(list1(elapsed));
	displaydat(of_string(" [ time: "));
	displaydat(secs);
	displaydat(of_string("s ] "));
}

static void save(enum reg reg)
{
	// Why use an enum?  Why not just pass the obj to save as an argument?
	//
	//      Because the call to newpair (below) allows garbage collection.
	//
	// So?  The argument obj wouldn't be reachable from the GC 'root' list.
	//
	// So?  If the arg(ument) is a pointer to a pair (i.e., is_pair), and a
	//      collection occurs, then the arg's pointer will not be updated
	//      because the garbage collector doesn't know about it.
	//
	// So?  The arg would still point to the pair's original location (now a
	//      'broken heart') and not to the pair's new location.
	//
	// Oh!  That's right, you just avoided putting corrupt data on the stack!
	//

	switch (reg) {
	case ARGL:
		stack = consgc(&argl, &stack);
		break;
	case CONT:
		stack = consgc(&cont, &stack);
		break;
	case ENV:
		stack = consgc(&env, &stack);
		break;
	case EXPR:
		stack = consgc(&expr, &stack);
		break;
	case PROC:
		stack = consgc(&proc, &stack);
		break;
	case STACK:
		stack = consgc(&stack, &stack);
		break;
	case UNEV:
		stack = consgc(&unev, &stack);
		break;
	case VAL:
		stack = consgc(&val, &stack);
		break;
	}
	if (is_err(stack)) {
		eprintf(AREA, "Halting - Reached Memory Limit");
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

// used for garbage collection
obj getroot(void)
{
	int actlen;
	obj lst = rootlst;

	// intentionally not using rootlen here, change number manually after
	// modifying body below.
	if ((actlen = length_u(rootlst)) != 9) {
		return error_internal(
			AREA,
			"Bug! getroot() got list of unexpected length: %d ",
			actlen);
	}
	// order must match setroot
	set_car(lst, argl);
	lst = cdr(lst);
	set_car(lst, cont);
	lst = cdr(lst);
	set_car(lst, env);
	lst = cdr(lst);
	set_car(lst, expr);
	lst = cdr(lst);
	set_car(lst, proc);
	lst = cdr(lst);
	set_car(lst, stack);
	lst = cdr(lst);
	set_car(lst, unev);
	lst = cdr(lst);
	set_car(lst, val);
	lst = cdr(lst);
	set_car(lst, the_global_environment());

	return rootlst;
}

// used for garbage collection
obj setroot(obj rlst)
{
	int actlen;
	obj lst = rootlst = rlst;

	// intentionally not using rootlen here, change number manually after
	// modifying body below.
	if ((actlen = length_u(rootlst)) != 9) {
		return error_internal(
			AREA,
			"Bug! setroot() got list of unexpected length: %d",
			actlen);
	}
	// order must match getroot
	argl = car(lst);
	lst = cdr(lst);
	cont = car(lst);
	lst = cdr(lst);
	env = car(lst);
	lst = cdr(lst);
	expr = car(lst);
	lst = cdr(lst);
	proc = car(lst);
	lst = cdr(lst);
	stack = car(lst);
	lst = cdr(lst);
	unev = car(lst);
	lst = cdr(lst);
	val = car(lst);
	lst = cdr(lst);
	set_global_environment(car(lst));

	return unspecified;
}

static obj proc_name;
static void set_proc_name(void)
{
	proc_name = is_symbol(expr) ?
			    expr :
			    (is_pair(expr) && is_eq(car(expr), lambda)) ?
			    lambda :
			    of_string("<unknown>");
}

static bool initdone = false;
static obj init(void)
{
	int actlen;

	stack = emptylst;
	// preallocate storage for gc root
	rootlst = listn(9,
			unspecified, // 1
			unspecified, // 2
			unspecified, // 3
			unspecified, // 4
			unspecified, // 5
			unspecified, // 6
			unspecified, // 7
			unspecified, // 8
			unspecified //  9
	);
	if ((actlen = length_u(rootlst)) != rootlen) {
		error_internal(
			AREA,
			"Bug! init, root wrong length. is: %d, expected %d",
			actlen, rootlen);
		exit(1);
	}
	initdone = true;
	return unspecified;
}

obj eval(obj expression, obj _environment)
{
	if (!initdone) {
		init();
	}
	expr = expression;
	env = _environment;
	cont = ev_return_caller;

// 5.4.1 The Core of the Evaluator

// ln 236
eval_dispatch:
	if (is_err(expr))
		return expr;

	if (is_self_evaluating(expr))
		goto ev_self_eval;
	if (is_variable(expr))
		goto ev_variable;
	if (is_quoted(expr))
		goto ev_quoted;
	if (is_assignment(expr))
		goto ev_assignment;
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
	if (is_let(expr))
		goto ev_let;
	if (is_and(expr))
		goto ev_and;
	if (is_or(expr))
		goto ev_or;
	if (is_delay(expr))
		goto ev_delay;
	if (is_cons_stream(expr))
		goto ev_cons_stream;
	if (is_time(expr))
		goto ev_timed;
	if (is_apply(expr))
		goto ev_apply;
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
ev_quoted:
	val = text_of_quotation(expr);
	goto go_cont;
ev_lambda:
	unev = lambda_parameters(expr);
	expr = lambda_body(expr);
	val = make_procedure(unev, expr, env);
	goto go_cont;

// Evaluating procedure applications

// ln 277
ev_application:
	save(CONT);
	save(ENV);
	unev = operands(expr);
	save(UNEV);
	expr = operator(expr);
	set_proc_name();
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
	save(PROC);

// ln 295
ev_appl_operand_loop:
	save(ARGL);
	expr = first_operand(unev);
	if (is_last_operand(unev))
		goto ev_appl_last_arg;
	save(ENV);
	save(UNEV);
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
	save(CONT);
	goto ev_sequence;

// ln 338
compound_apply:
	unev = procedure_parameters(proc);
	env = procedure_environment(proc);
	env = extend_environment(unev, argl, env, proc_name);
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
	save(UNEV);
	save(ENV);
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
	save(EXPR); // save expression for later
	save(ENV);
	save(CONT);
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

// ln 399
ev_assignment:
	unev = assignment_variable(expr);
	save(UNEV); // save variable for later
	expr = assignment_value(expr);
	save(ENV);
	save(CONT);
	cont = ev_assignment_1;
	goto eval_dispatch; // evaluate the assignment value

ev_assignment_1:
	cont = restore();
	env = restore();
	unev = restore();
	set_variable_value(unev, val, env);
	val = ok;
	goto go_cont;

// ln 416
ev_definition:
	unev = definition_variable(expr);
	save(UNEV); // save variable for later
	expr = definition_value(expr);
	save(ENV);
	save(CONT);
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

// new - let
ev_let:
	expr = let_to_combination(expr);
	goto eval_dispatch;

// new - and
ev_and:
	save(CONT);
	unev = operands(expr);

ev_and_loop:
	if (!no_operands(unev))
		goto ev_and_operand;
	val = true_o;
	cont = restore();
	goto go_cont;

ev_and_operand:
	expr = first_operand(unev);
	unev = rest_operands(unev);
	save(ENV);
	save(UNEV);
	cont = ev_and_test;
	goto eval_dispatch;

ev_and_test:
	unev = restore();
	env = restore();
	if (is_true(val))
		goto ev_and_loop;
	cont = restore();
	goto go_cont;

// new - or
ev_or:
	save(CONT);
	unev = operands(expr);

ev_or_loop:
	if (!no_operands(unev))
		goto ev_or_operand;
	val = false_o;
	cont = restore();
	goto go_cont;

ev_or_operand:
	expr = first_operand(unev);
	unev = rest_operands(unev);
	save(ENV);
	save(UNEV);
	cont = ev_or_test;
	goto eval_dispatch;

ev_or_test:
	unev = restore();
	env = restore();
	if (is_false(val))
		goto ev_or_loop;
	val = true_o;
	cont = restore();
	goto go_cont;

// new - delay / cons-stream
ev_delay:
	expr = delay_to_lambda(expr);
	goto eval_dispatch;

ev_cons_stream:
	expr = cons_stream_to_cons(expr);
	goto eval_dispatch;

// new
ev_timed:
	save(UNEV);
	unev = runtime(emptylst);
	save(UNEV);
	save(CONT);
	cont = ev_timed_done;
	expr = cons(begin, cdr(expr));
	goto eval_dispatch;

ev_timed_done:
	cont = restore();
	unev = restore();
	timed_eval(unev);
	unev = restore();
	goto go_cont;

// new
ev_apply:
	save(CONT);
	save(ENV);
	unev = apply_operands(expr);
	save(UNEV);
	expr = apply_operator(expr);
	cont = ev_apply_2;
	goto eval_dispatch;

ev_apply_2:
	unev = restore();
	env = restore();
	proc = val;
	save(PROC);
	save(ENV);
	expr = unev;
	cont = ev_apply_3;
	goto eval_dispatch;

ev_apply_3:
	env = restore();
	proc = restore();
	argl = val;
	goto apply_dispatch;

// ln 433
unknown_expression_type:
	return error_eval(AREA, "Unknown expression type: %s", errstr(expr));
unknown_procedure_type:
	cont = restore(); // clean up stack (from apply-dispatch)
	return error_eval(AREA, "Unknown procedure type: %s", errstr(proc));

go_cont:
	if (is_eq(cont, ev_return_caller))
		return val;
	if (is_eq(cont, ev_and))
		goto ev_and;
	if (is_eq(cont, ev_and_loop))
		goto ev_and_loop;
	if (is_eq(cont, ev_and_operand))
		goto ev_and_operand;
	if (is_eq(cont, ev_and_test))
		goto ev_and_test;
	if (is_eq(cont, ev_appl_accum_last_arg))
		goto ev_appl_accum_last_arg;
	if (is_eq(cont, ev_appl_accumulate_arg))
		goto ev_appl_accumulate_arg;
	if (is_eq(cont, ev_appl_did_operator))
		goto ev_appl_did_operator;
	if (is_eq(cont, ev_apply_2))
		goto ev_apply_2;
	if (is_eq(cont, ev_apply_3))
		goto ev_apply_3;
	if (is_eq(cont, ev_assignment_1))
		goto ev_assignment_1;
	if (is_eq(cont, ev_definition_1))
		goto ev_definition_1;
	if (is_eq(cont, ev_if_decide))
		goto ev_if_decide;
	if (is_eq(cont, ev_or))
		goto ev_or;
	if (is_eq(cont, ev_or_loop))
		goto ev_or_loop;
	if (is_eq(cont, ev_or_operand))
		goto ev_or_operand;
	if (is_eq(cont, ev_or_test))
		goto ev_or_test;
	if (is_eq(cont, ev_quoted))
		goto ev_quoted;
	if (is_eq(cont, ev_sequence_continue))
		goto ev_sequence_continue;
	if (is_eq(cont, ev_timed_done))
		goto ev_timed_done;
	return error_internal(AREA, "BUG! Goto... where?: %s", to_string(cont));
}
