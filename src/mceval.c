/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/mc-evaluator-50.scm */

#include "mceval.h"

#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "list.h"

#define AREA "MC-EVAL"

static obj make_lambda(obj, obj);
static obj make_if(obj predicate, obj consequent, obj alternative);
static obj sequence_to_exp(obj seq);
static obj make_begin(obj seq);
static obj cond_clauses(obj);
static bool is_cond_else_clause(obj);
static obj cond_predicate(obj);
static obj cond_actions(obj);
static obj expand_clauses(obj);

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

// ln 122
static bool is_tagged_list(obj exp, obj tag)
{
	return is_pair(exp) ? eq_symbol(car(exp), tag) : false;
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
		return fls;
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
obj cond_to_if(obj exp)
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

// new
bool is_time(obj exp)
{
	return is_tagged_list(exp, time_s);
}

obj timed_expr(obj exp)
{
	return cadr(exp);
}
