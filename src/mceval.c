/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/mc-evaluator-50.scm */

#include "mceval.h"

#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "list.h"
#include "primproc.h"

#define AREA "MC-EVAL"

static bool is_tagged_list(obj, obj);
static obj make_lambda(obj, obj);
static obj make_if(obj predicate, obj consequent, obj alternative);
static obj sequence_to_exp(obj seq);
static obj make_begin(obj seq);
static obj cond_clauses(obj);
static bool is_cond_else_clause(obj);
static obj cond_predicate(obj);
static obj cond_actions(obj);
static obj expand_clauses(obj);
static obj make_proc_call(obj proc, obj args);

// ln 110
bool is_self_evaluating(obj exp)
{
	return is_number(exp) || is_string(exp);
}

// ln 115
bool is_variable(obj exp)
{
	return is_symbol(exp);
}

// ln 117
bool is_quoted(obj exp)
{
	return is_tagged_list(exp, quote);
}

// ln 120
obj text_of_quotation(obj exp)
{
	return cadr(exp);
}

// ln 122
static bool is_tagged_list(obj exp, obj tag)
{
	return is_pair(exp) ? is_eq(car(exp), tag) : false;
}

// ln 127
bool is_assignment(obj exp)
{
	return is_tagged_list(exp, set);
}

// ln 129
obj assignment_variable(obj exp)
{
	return cadr(exp);
}

// ln 130
obj assignment_value(obj exp)
{
	return caddr(exp);
}

// ln 132
bool is_definition(obj exp)
{
	return is_tagged_list(exp, define);
}

// ln 134
obj definition_variable(obj exp)
{
	return is_symbol(cadr(exp)) ? cadr(exp) : caadr(exp);
}

// ln 138
obj definition_value(obj exp)
{
	return is_symbol(cadr(exp)) ?
		       caddr(exp) :
		       make_lambda(cdadr(exp), // formal parameters
				   cddr(exp)); // body
}

// ln 144
bool is_lambda(obj exp)
{
	return is_tagged_list(exp, lambda);
}

// ln 145
obj lambda_parameters(obj exp)
{
	return cadr(exp);
}

// ln 146
obj lambda_body(obj exp)
{
	return cddr(exp);
}

// ln 148
static obj make_lambda(obj parameters, obj body)
{
	return cons(lambda, cons(parameters, body));
}

// ln 151
bool is_if(obj exp)
{
	return is_tagged_list(exp, if_s);
}

// ln 152
obj if_predicate(obj exp)
{
	return cadr(exp);
}

// ln 153
obj if_consequent(obj exp)
{
	return caddr(exp);
}

// ln 155
obj if_alternate(obj exp)
{
	if (!is_null(cdddr(exp))) {
		return cadddr(exp);
	} else {
		return false_s;
	}
}

// ln 160
static obj make_if(obj predicate, obj consequent, obj alternative)
{
	return list4(if_s, predicate, consequent, alternative);
}

// ln 163
bool is_begin(obj exp)
{
	return is_tagged_list(exp, begin);
}

// ln 164
obj begin_actions(obj exp)
{
	return cdr(exp);
}

// ln 165
bool is_last_exp(obj seq)
{
	return is_null(cdr(seq));
}

// ln 166
obj first_exp(obj seq)
{
	return car(seq);
}

// ln 167
obj rest_exps(obj seq)
{
	return cdr(seq);
}

// ln 169
static obj sequence_to_exp(obj seq)
{
	if (is_null(seq))
		return seq;
	else if (is_last_exp(seq))
		return first_exp(seq);
	else
		return make_begin(seq);
}

// ln 173
static obj make_begin(obj seq)
{
	return cons(begin, seq);
}

// ln 175
bool is_application(obj exp)
{
	return is_pair(exp);
}

// ln 176
obj operator(obj exp)
{
	return car(exp);
}

// ln 177
obj operands(obj exp)
{
	return cdr(exp);
}

// ln 178
bool no_operands(obj ops)
{
	return is_null(ops);
}

// ln 179
obj first_operand(obj ops)
{
	return car(ops);
}

// ln 180
obj rest_operands(obj ops)
{
	return cdr(ops);
}

// ln 184
bool is_cond(obj exp)
{
	return is_tagged_list(exp, cond);
}

// ln 185
static obj cond_clauses(obj exp)
{
	return cdr(exp);
}

// ln 186
static bool is_cond_else_clause(obj clause)
{
	return is_eq(cond_predicate(clause), else_s);
}

// ln 188
static obj cond_predicate(obj clause)
{
	return car(clause);
}

// ln 189
static obj cond_actions(obj clause)
{
	return cdr(clause);
}

// new - recipient clause
static bool is_cond_recipient_clause(obj exp)
{
	return is_tagged_list(cdr(exp), arrow);
}

static obj cond_recipient(obj exp)
{
	return caddr(exp);
}

static obj cond_expand_recipient_clause(obj clause, obj rest)
{
	obj pv = of_identifier("pred_value");
	return make_proc_call(
		make_lambda(list1(pv),
			    list1(make_if(pv,
					  make_proc_call(cond_recipient(clause),
							 list1(pv)),
					  expand_clauses(rest)))),
		list1(cond_predicate(clause)));
}

// ln 190
obj cond_to_if(obj exp)
{
	return expand_clauses(cond_clauses(exp));
}

// ln 193
static obj expand_clauses(obj clauses)
{
	if (is_null(clauses))
		return false_s; // no else clause
	obj first = car(clauses);
	obj rest = cdr(clauses);
	if (is_cond_else_clause(first)) {
		if (is_null(rest))
			return sequence_to_exp(cond_actions(first));
		else
			return error_syntax(AREA, "ELSE clause isn't last: %s",
					    errstr(clauses));
	} else {
		if (is_cond_recipient_clause(first)) {
			return cond_expand_recipient_clause(first, rest);
		} else {
			return make_if(cond_predicate(first),
				       sequence_to_exp(cond_actions(first)),
				       expand_clauses(rest));
		}
	}
}

// ln 222
obj make_procedure(obj parameters, obj body, obj env)
{
	return list4(procedure, parameters, body, env);
}

// ln 223
bool is_compound_procedure(obj exp)
{
	return is_tagged_list(exp, procedure);
}

// ln 225
obj procedure_parameters(obj p)
{
	return cadr(p);
}

// ln 226
obj procedure_body(obj p)
{
	return caddr(p);
}

// ln 227
obj procedure_environment(obj p)
{
	return cadddr(p);
}

/**********************************
 *                                *
 * lns 233-317, see environment.c *
 *                                *
 **********************************/

// ln 324
obj apply_primitive_procedure(obj proc, obj args)
{
	return (to_function(proc))(args);
}

// new - let->combination
static obj make_defintion(obj variable, obj value)
{
	return cons(define, cons(variable, value));
}

static obj make_proc_call(obj proc, obj args)
{
	return cons(proc, args);
}

static obj make_let(obj bindings, obj body)
{
	return cons(let, cons(bindings, body));
}

bool is_let(obj exp)
{
	return is_tagged_list(exp, let);
}

static bool is_named_let(obj exp)
{
	return is_pair(exp) && is_pair(exp = cdr(exp)) && is_symbol(car(exp));
}

static obj let_bindings(obj exp)
{
	return is_named_let(exp) ? caddr(exp) : cadr(exp);
}

static obj let_body(obj exp)
{
	return is_named_let(exp) ? cdddr(exp) : cddr(exp);
}

static obj let_var(obj binding)
{
	return car(binding);
}

static obj let_val(obj binding)
{
	return cadr(binding);
}

static obj let_variables(obj exp)
{
	return map_u(let_var, let_bindings(exp));
}

static obj let_values(obj exp)
{
	return map_u(let_val, let_bindings(exp));
}

obj let_to_combination(obj exp)
{
	if (is_named_let(exp)) {
		obj name = cadr(exp);
		obj defn = make_defintion(cons(name, let_variables(exp)),
					  let_body(exp));
		obj call = make_proc_call(name, let_values(exp));
		exp = make_let(emptylst, list2(defn, call));
	}
	return make_proc_call(make_lambda(let_variables(exp), let_body(exp)),
			      (let_values(exp)));
}

// new - let*->nested-lets

bool is_letstar(obj exp)
{
	return is_tagged_list(exp, letstar);
}

obj letstar_to_combination(obj exp)
{
	obj binds = let_bindings(exp);
	obj body = let_body(exp);

	if (is_null(binds)) {
		exp = make_let(emptylst, body);
	} else {
		for (binds = reverse(binds); is_pair(binds);
		     binds = cdr(binds)) {
			body = list1(make_let(list1(car(binds)), body));
		}
		exp = car(body);
	}
	return let_to_combination(exp);
}

// new - letrec
bool is_letrec(obj exp)
{
	return is_tagged_list(exp, letrec);
}

obj letrec_to_combination(obj exp)
{
	obj bnds = let_bindings(exp);
	obj body = let_body(exp);
	for (bnds = reverse(bnds); is_pair(bnds); bnds = cdr(bnds)) {
		obj bnd = car(bnds);
		body = cons(list3(set, let_var(bnd), let_val(bnd)), body);
	}
	bnds = let_bindings(exp);
	obj args;
	for (args = emptylst; is_pair(bnds); bnds = cdr(bnds)) {
		args = cons(list2(quote, unassigned), args);
	}
	return make_proc_call(make_lambda(let_variables(exp), body), args);
}

// new - and
bool is_and(obj exp)
{
	return is_tagged_list(exp, and_s);
}

obj and_to_if(obj exp)
{
	obj vals = cdr(exp);
	exp = is_null(vals) ?
		      true_s :
		      list4(if_s, car(vals), cons(and_s, cdr(vals)), false_s);
	return exp;
}

// new - or
bool is_or(obj exp)
{
	return is_tagged_list(exp, or_s);
}

obj or_to_if(obj exp)
{
	obj vals = cdr(exp);
	exp = is_null(vals) ?
		      false_s :
		      list4(if_s, car(vals), true_s, cons(or_s, cdr(vals)));
	return exp;
}

// new - time expression's evaluation
bool is_time(obj exp)
{
	return is_tagged_list(exp, time_s);
}

obj timed_expr(obj exp)
{
	return cadr(exp);
}

// apply keyword
bool is_ecapply(obj exp)
{
	return is_tagged_list(exp, __ppapply);
}

bool is_anapply(obj exp)
{
	return is_tagged_list(exp, uapply);
}

obj apply_operator(obj exp)
{
	return cadr(exp);
}

obj apply_operands(obj exp)
{
	return caddr(exp);
}

// new - delay

bool is_delay(obj exp)
{
	return is_tagged_list(exp, delay);
}

obj delay_to_lambda(obj exp)
{
	obj r = make_lambda(emptylst, cdr(exp));
	return r;
}

bool is_cons_stream(obj exp)
{
	return is_tagged_list(exp, cons_stream);
}

obj cons_stream_to_cons(obj exp)
{
	return list3(cons_s, cadr(exp),
		     list2(memo_proc,
			   delay_to_lambda(list2(delay, caddr(exp)))));
}

bool is_amb(obj exp)
{
	return is_tagged_list(exp, amb);
}
