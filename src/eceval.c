/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/ec-evaluator-00.scm */

#include "eceval.h"

#include <stdlib.h>
#include "error.h"
#include "list.h"
#include "mceval.h"
#include "output.h"
#include "primproc.h"
#include "storage.h"

#define AREA "ECEVAL"

struct core {
	obj argl; //     1
	obj cont; //     2
	obj env; //      3
	obj expr; //     4
	obj proc; //     5
	obj stack; //    6
	obj unev; //     7
	obj val; //      8
} cre;
struct core *cr0 = &cre;

// the_global_environment // 9
obj anenv; //   10
obj ambenv; //  11
obj savetmp; // 12

const int rootlen = 12;
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

static void save(obj dat, struct core *cr)
{
	savetmp = dat;
	cr->stack = consgc(&savetmp, &cr->stack);

	if (is_err(cr->stack)) {
		eprintf(AREA, "Halting - Reached Memory Limit");
		exit(1);
	}
	return;
}

static obj restore(struct core *cr)
{
	if (is_null(cr->stack)) {
		eprintf(AREA, "attempted to restore from empty cr->stack");
		exit(1);
	} else {
		obj dat = car(cr->stack);
		cr->stack = cdr(cr->stack);
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
	if ((actlen = length_u(rootlst)) != 12) {
		error_internal(
			AREA,
			"Bug! getroot() got list of unexpected length: %d ",
			actlen);
		exit(1);
	}
	// order must match setroot
	set_car(lst, cr0->argl);
	lst = cdr(lst);
	set_car(lst, cr0->cont);
	lst = cdr(lst);
	set_car(lst, cr0->env);
	lst = cdr(lst);
	set_car(lst, cr0->expr);
	lst = cdr(lst);
	set_car(lst, cr0->proc);
	lst = cdr(lst);
	set_car(lst, cr0->stack);
	lst = cdr(lst);
	set_car(lst, cr0->unev);
	lst = cdr(lst);
	set_car(lst, cr0->val);
	lst = cdr(lst);
	set_car(lst, the_global_environment());
	lst = cdr(lst);
	set_car(lst, anenv);
	lst = cdr(lst);
	set_car(lst, ambenv);
	lst = cdr(lst);
	set_car(lst, savetmp);

	return rootlst;
}

// used for garbage collection
obj setroot(obj rlst)
{
	int actlen;
	obj lst = rootlst = rlst;

	// intentionally not using rootlen here, change number manually after
	// modifying body below.
	if ((actlen = length_u(rootlst)) != 12) {
		return error_internal(
			AREA,
			"Bug! setroot() got list of unexpected length: %d",
			actlen);
	}
	// order must match getroot
	cr0->argl = car(lst);
	lst = cdr(lst);
	cr0->cont = car(lst);
	lst = cdr(lst);
	cr0->env = car(lst);
	lst = cdr(lst);
	cr0->expr = car(lst);
	lst = cdr(lst);
	cr0->proc = car(lst);
	lst = cdr(lst);
	cr0->stack = car(lst);
	lst = cdr(lst);
	cr0->unev = car(lst);
	lst = cdr(lst);
	cr0->val = car(lst);
	lst = cdr(lst);
	set_global_environment(car(lst));
	lst = cdr(lst);
	anenv = car(lst);
	lst = cdr(lst);
	ambenv = car(lst);
	lst = cdr(lst);
	savetmp = car(lst);

	return unspecified;
}

static obj proc_name;
static void set_proc_name(struct core *cr)
{
	proc_name = is_symbol(cr->expr) ? cr->expr :
					  (is_pair(cr->expr) &&
					   is_eq(car(cr->expr), lambda)) ?
					  lambda :
					  of_string("<unknown>");
}

static bool initdone = false;
static obj init(void)
{
	int actlen;

	cr0->stack = emptylst;
	// preallocate storage for gc root
	rootlst = listn(12, //       <----- actual  length
			unspecified, //  1
			unspecified, //  2
			unspecified, //  3
			unspecified, //  4
			unspecified, //  5
			unspecified, //  6
			unspecified, //  7
			unspecified, //  8
			unspecified, //  9
			unspecified, // 10
			unspecified, // 11
			unspecified //  12
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

static obj ecevalcr(obj expression, obj _environment, struct core *cr)
{
	if (!initdone) {
		init();
	}
	cr->expr = expression;
	cr->env = _environment;
	cr->cont = ev_return_caller;

// 5.4.1 The Core of the Evaluator

// ln 236
eval_dispatch:
	if (is_err(cr->expr))
		return cr->expr;

	// printf("eval: ");
	// printf("%s\n", errstr(cr->expr));

	if (is_self_evaluating(cr->expr))
		goto ev_self_eval;
	if (is_variable(cr->expr))
		goto ev_variable;
	if (is_quoted(cr->expr))
		goto ev_quoted;
	if (is_quasiquote(cr->expr))
		goto ev_quasiquoted;
	if (is_assignment(cr->expr))
		goto ev_assignment;
	if (is_definition(cr->expr))
		goto ev_definition;
	if (is_if(cr->expr))
		goto ev_if;
	if (is_lambda(cr->expr))
		goto ev_lambda;
	if (is_begin(cr->expr))
		goto ev_begin;
	if (is_cond(cr->expr))
		goto ev_cond;
	if (is_let(cr->expr))
		goto ev_let;
	if (is_letrec(cr->expr))
		goto ev_letrec;
	if (is_letstar(cr->expr))
		goto ev_letstar;
	if (is_and(cr->expr))
		goto ev_and;
	if (is_or(cr->expr))
		goto ev_or;
	if (is_delay(cr->expr))
		goto ev_delay;
	if (is_cons_stream(cr->expr))
		goto ev_cons_stream;
	if (is_time(cr->expr))
		goto ev_timed;
	if (is_ecapply(cr->expr))
		goto ev_apply;
	if (is_application(cr->expr))
		goto ev_application;
	goto unknown_expression_type;

// Evaluating Simple Expressions

// ln 259
ev_self_eval:
	cr->val = cr->expr;
	goto go_cont;
ev_variable:
	cr->val = lookup_variable_value(cr->expr, cr->env);
	if (is_err(cr->val))
		return cr->val;
	goto go_cont;
ev_quoted:
	cr->val = text_of_quotation(cr->expr);
	goto go_cont;
ev_quasiquoted:
	cr->expr = quasi_to_combination(cr->expr);
	goto eval_dispatch;
ev_lambda:
	cr->unev = lambda_parameters(cr->expr);
	cr->expr = lambda_body(cr->expr);
	cr->val = make_procedure(cr->unev, cr->expr, cr->env);
	goto go_cont;

// Evaluating procedure applications

// ln 277
ev_application:
	save(cr->cont, cr);
	save(cr->env, cr);
	cr->unev = operands(cr->expr);
	save(cr->unev, cr);
	cr->expr = operator(cr->expr);
	set_proc_name(cr);
	cr->cont = ev_appl_did_operator;
	goto eval_dispatch;

// ln 286
ev_appl_did_operator:
	cr->unev = restore(cr); // the operands
	cr->env = restore(cr);
	cr->argl = empty_arglist();
	cr->proc = cr->val; // the operator
	if (is_err(cr->proc))
		return cr->proc;
	if (no_operands(cr->unev))
		goto apply_dispatch;
	save(cr->proc, cr);

// ln 295
ev_appl_operand_loop:
	save(cr->argl, cr);
	cr->expr = first_operand(cr->unev);
	if (is_last_operand(cr->unev))
		goto ev_appl_last_arg;
	save(cr->env, cr);
	save(cr->unev, cr);
	cr->cont = ev_appl_accumulate_arg;
	goto eval_dispatch;

// ln 305
ev_appl_accumulate_arg:
	cr->unev = restore(cr);
	cr->env = restore(cr);
	cr->argl = restore(cr);
	cr->argl = adjoin_arg(cr->val, cr->argl);
	cr->unev = rest_operands(cr->unev);
	goto ev_appl_operand_loop;

// ln 313
ev_appl_last_arg:
	cr->cont = ev_appl_accum_last_arg;
	goto eval_dispatch;
ev_appl_accum_last_arg:
	cr->argl = restore(cr);
	cr->argl = adjoin_arg(cr->val, cr->argl);
	cr->proc = restore(cr);
	goto apply_dispatch;

// Procedure application

// ln 324
apply_dispatch:
	if (is_primitive_procedure(cr->proc))
		goto primitive_apply;
	if (is_compound_procedure(cr->proc))
		goto compound_apply;
	goto unknown_procedure_type;

// ln 331
primitive_apply:
	cr->val = apply_primitive_procedure(cr->proc, cr->argl);
	if (is_err(cr->val))
		return cr->val;
	cr->cont = restore(cr);
	goto go_cont;

// ln 348
ev_begin:
	cr->unev = begin_actions(cr->expr);
	save(cr->cont, cr);
	goto ev_sequence;

// ln 338
compound_apply:
	cr->unev = procedure_parameters(cr->proc);
	cr->env = procedure_environment(cr->proc);
	cr->env = extend_environment(cr->unev, cr->argl, cr->env, proc_name);
	if (is_err(cr->env))
		return cr->env;
	cr->unev = procedure_body(cr->proc);
	goto ev_sequence;

// 5.4.2 Sequence Evaluation and Tail Recursion

// ln 353
ev_sequence:
	cr->expr = first_exp(cr->unev);
	if (is_last_exp(cr->unev))
		goto ev_sequence_last_exp;
	save(cr->unev, cr);
	save(cr->env, cr);
	cr->cont = ev_sequence_continue;
	goto eval_dispatch;
ev_sequence_continue:
	cr->env = restore(cr);
	cr->unev = restore(cr);
	cr->unev = rest_exps(cr->unev);
	goto ev_sequence;
ev_sequence_last_exp:
	cr->cont = restore(cr);
	goto eval_dispatch;

// 5.4.3 Conditionals, Assignments and Definitions

// ln 374
ev_if:
	save(cr->expr, cr); // save expression for later
	save(cr->env, cr);
	save(cr->cont, cr);
	cr->cont = ev_if_decide;
	cr->expr = if_predicate(cr->expr);
	goto eval_dispatch; // evaluate the predicate

// ln 382
ev_if_decide:
	cr->cont = restore(cr);
	cr->env = restore(cr);
	cr->expr = restore(cr);
	if (is_true(cr->val))
		goto ev_if_consequent;

	// ln 389
	//ev_if_alternative:
	cr->expr = if_alternate(cr->expr);
	goto eval_dispatch;
ev_if_consequent:
	cr->expr = if_consequent(cr->expr);
	goto eval_dispatch;

// ln 399
ev_assignment:
	cr->unev = assignment_variable(cr->expr);
	save(cr->unev, cr); // save variable for later
	cr->expr = assignment_value(cr->expr);
	save(cr->env, cr);
	save(cr->cont, cr);
	cr->cont = ev_assignment_1;
	goto eval_dispatch; // evaluate the assignment value

ev_assignment_1:
	cr->cont = restore(cr);
	cr->env = restore(cr);
	cr->unev = restore(cr);
	set_variable_value(cr->unev, cr->val, cr->env);
	cr->val = ok;
	goto go_cont;

// ln 416
ev_definition:
	cr->unev = definition_variable(cr->expr);
	save(cr->unev, cr); // save variable for later
	cr->expr = definition_value(cr->expr);
	save(cr->env, cr);
	save(cr->cont, cr);
	cr->cont = ev_definition_1;
	goto eval_dispatch; // evaluate the definition value
ev_definition_1:
	cr->cont = restore(cr);
	cr->env = restore(cr);
	cr->unev = restore(cr);
	define_variable(cr->unev, cr->val, cr->env);
	cr->val = ok;
	goto go_cont;

ev_cond:
	cr->expr = cond_to_if(cr->expr);
	goto eval_dispatch;

// new - let
ev_let:
	cr->expr = let_to_combination(cr->expr);
	goto ev_application;

// new - letrec
ev_letrec:
	cr->expr = letrec_to_combination(cr->expr);
	goto ev_application;

// new - letstar
ev_letstar:
	cr->expr = letstar_to_combination(cr->expr);
	goto ev_application;

// new - and
ev_and:
	cr->expr = and_to_if(cr->expr);
	goto eval_dispatch;

// new - or
ev_or:
	cr->expr = or_to_if(cr->expr);
	goto eval_dispatch;

// new - delay / cons-stream
ev_delay:
	cr->expr = delay_to_lambda(cr->expr);
	goto ev_lambda;

ev_cons_stream:
	cr->expr = cons_stream_to_cons(cr->expr);
	goto ev_application;

// new
ev_timed:
	save(cr->unev, cr);
	cr->unev = runtime(emptylst);
	save(cr->unev, cr);
	save(cr->cont, cr);
	cr->cont = ev_timed_done;
	cr->expr = cons(begin, cdr(cr->expr));
	goto eval_dispatch;

ev_timed_done:
	cr->cont = restore(cr);
	cr->unev = restore(cr);
	timed_eval(cr->unev);
	cr->unev = restore(cr);
	goto go_cont;

// new
ev_apply:
	save(cr->cont, cr);
	save(cr->env, cr);
	cr->unev = apply_operands(cr->expr);
	save(cr->unev, cr);
	cr->expr = apply_operator(cr->expr);
	cr->cont = ev_apply_2;
	goto eval_dispatch;

ev_apply_2:
	cr->unev = restore(cr);
	cr->env = restore(cr);
	cr->proc = cr->val;
	save(cr->proc, cr);
	save(cr->env, cr);
	cr->expr = cr->unev;
	cr->cont = ev_apply_3;
	goto eval_dispatch;

ev_apply_3:
	cr->env = restore(cr);
	cr->proc = restore(cr);
	cr->argl = cr->val;
	goto apply_dispatch;

// ln 433
unknown_expression_type:
	return error_eval(AREA, "Unknown expression type: %s",
			  errstr(cr->expr));
unknown_procedure_type:
	cr->cont = restore(cr); // clean up cr->stack (from apply-dispatch)
	return error_eval(AREA, "Unknown procedure type: %s", errstr(cr->proc));

go_cont:
	if (is_eq(cr->cont, ev_return_caller))
		return cr->val;
	if (is_eq(cr->cont, ev_appl_accum_last_arg))
		goto ev_appl_accum_last_arg;
	if (is_eq(cr->cont, ev_appl_accumulate_arg))
		goto ev_appl_accumulate_arg;
	if (is_eq(cr->cont, ev_appl_did_operator))
		goto ev_appl_did_operator;
	if (is_eq(cr->cont, ev_apply_2))
		goto ev_apply_2;
	if (is_eq(cr->cont, ev_apply_3))
		goto ev_apply_3;
	if (is_eq(cr->cont, ev_assignment_1))
		goto ev_assignment_1;
	if (is_eq(cr->cont, ev_definition_1))
		goto ev_definition_1;
	if (is_eq(cr->cont, ev_if_decide))
		goto ev_if_decide;
	if (is_eq(cr->cont, ev_quoted))
		goto ev_quoted;
	if (is_eq(cr->cont, ev_sequence_continue))
		goto ev_sequence_continue;
	if (is_eq(cr->cont, ev_timed_done))
		goto ev_timed_done;
	return error_internal(AREA, "BUG! Goto... where?: %s",
			      to_string(cr->cont));
}

obj eceval(obj expression, obj _environment)
{
	return ecevalcr(expression, _environment,cr0);
}
