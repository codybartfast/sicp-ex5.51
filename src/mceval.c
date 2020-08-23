/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/mc-evaluator-50.scm */

#include "mceval.h"

#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "list.h"
#include "output.h"

#define AREA "EVAL"

static obj apply(obj procedure, obj arguments);
static obj list_of_values(obj exps, obj env);
static obj eval_if(obj exp, obj env);
static obj eval_sequence(obj exps, obj env);
static obj eval_definition(obj exp, obj env);
static obj definition_variable(obj exp);
static obj definition_value(obj exp);
static obj make_lambda(obj, obj);
static obj if_predicate(obj);
static obj if_consequent(obj);
static obj if_alternate(obj);
static obj make_if(obj predicate, obj consequent, obj alternative);
static bool is_last_exp(obj seq);
static obj first_exp(obj seq);
static obj rest_exps(obj seq);
static obj sequence_to_exp(obj seq);
static obj make_begin(obj seq);
static obj rest_operands(obj ops);
static bool is_cond(obj);
static obj cond_clauses(obj);
static bool is_cond_else_clause(obj);
static obj cond_predicate(obj);
static obj cond_actions(obj);
static obj cond_to_if(obj);
static obj expand_clauses(obj);
static obj procedure_environment(obj p);
static obj apply_primitive_procedure(obj proc, obj args);

static obj eval(obj exp, obj env)
{
	if (is_err(exp))
		return exp;

	if (is_self_evaluating(exp))
		return exp;
	if (is_variable(exp))
		return lookup_variable_value(exp, env);
	if (is_definition(exp))
		return eval_definition(exp, env);
	if (is_if(exp))
		return eval_if(exp, env);
	if (is_lambda(exp))
		return make_procedure(lambda_parameters(exp), lambda_body(exp),
				      env);
	if (is_cond(exp))
		return eval(cond_to_if(exp), env);
	if (is_application(exp)) {
		obj proc, args;
		proc = eval(operator(exp), env);
		if (is_err(proc))
			return proc;
		args = list_of_values(operands(exp), env);
		if (is_err(args))
			return args;
		return apply(proc, args);
	}
	return error_eval(AREA, "Unknown expression type: %s", errstr(exp));
}

static obj apply(obj procedure, obj arguments)
{
	if (is_err(procedure))
		return procedure;

	if (is_primproc(procedure)) {
		return apply_primitive_procedure(procedure, arguments);
	} else if (is_compound_procedure(procedure)) {
		return eval_sequence(
			procedure_body(procedure),
			extend_environment(procedure_parameters(procedure),
					   arguments,
					   procedure_environment(procedure)));
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

// ln 80
static obj eval_if(obj exp, obj env)
{
	obj p = eval(if_predicate(exp), env);
	if (is_err(p))
		return p;
	if (is_true(p)) {
		return eval(if_consequent(exp), env);
	} else {
		return eval(if_alternate(exp), env);
	}
}

// ln 85
static obj eval_sequence(obj exps, obj env)
{
	if (is_last_exp(exps)) {
		return eval(first_exp(exps), env);
	} else {
		eval(first_exp(exps), env);
		return eval_sequence(rest_exps(exps), env);
	}
}

// ln 100
static obj eval_definition(obj exp, obj env)
{
	obj r = define_variable(definition_variable(exp),
				eval(definition_value(exp), env), env);
	return is_err(r) ? r : ok;
}

// ln 110
bool is_self_evaluating(obj exp)
{
	return is_number(exp);
}

// ln 115
bool is_variable(obj exp)
{
	return is_symbol(exp);
}

// ln 122
static bool is_tagged_list(obj exp, obj tag)
{
	return is_pair(exp) ? eq_symbol(car(exp), tag) : false;
}

// ln 132
bool is_definition(obj exp)
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

// ln 144
static bool is_lambda(obj exp)
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
static obj if_predicate(obj exp)
{
	return cadr(exp);
}

// ln 153
static obj if_consequent(obj exp)
{
	return caddr(exp);
}

// ln 155
static obj if_alternate(obj exp)
{
	if (!is_null(cdddr(exp))) {
		return cadddr(exp);
	} else {
		return fls;
	}
}

// ln 160
static obj make_if(obj predicate, obj consequent, obj alternative)
{
	return list4(if_s, predicate, consequent, alternative);
}

// ln 165
static bool is_last_exp(obj seq)
{
	return is_null(cdr(seq));
}

// ln 166
static obj first_exp(obj seq)
{
	return car(seq);
}

// ln 167
static obj rest_exps(obj seq)
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
static bool is_cond(obj exp)
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
	return eq_symbol(cond_predicate(clause), else_s);
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

// ln 190
static obj cond_to_if(obj exp)
{
	return expand_clauses(cond_clauses(exp));
}

// ln 193
static obj expand_clauses(obj clauses)
{
	if (is_null(clauses))
		return fls; // no else clause
	obj first = car(clauses);
	obj rest = cdr(clauses);
	if (is_cond_else_clause(first)) {
		if (is_null(rest))
			return sequence_to_exp(cond_actions(first));
		else
			return error_syntax(AREA, "ELSE clause isn't last: %s",
					    errstr(clauses));
	} else {
		return make_if(cond_predicate(first),
			       sequence_to_exp(cond_actions(first)),
			       expand_clauses(rest));
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
static obj procedure_environment(obj p)
{
	return cadddr(p);
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
