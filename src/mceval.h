#ifndef MCEVAL_H
#define MCEVAL_H
#include "sicpstd.h"

#include "environment.h"
#include "obj.h"

bool is_self_evaluating(obj exp);
bool is_variable(obj exp);
bool is_quoted(obj exp);
obj text_of_quotation(obj exp);
bool is_assignment(obj exp);
obj assignment_variable(obj exp);
obj assignment_value(obj exp);
bool is_definition(obj exp);
bool is_lambda(obj exp);
obj lambda_parameters(obj exp);
obj lambda_body(obj exp);
obj definition_variable(obj exp);
obj definition_value(obj exp);
bool is_if(obj);
obj if_predicate(obj);
obj if_consequent(obj);
obj if_alternate(obj);
bool is_begin(obj);
obj begin_actions(obj exp);
bool is_last_exp(obj seq);
obj first_exp(obj seq);
obj rest_exps(obj seq);
bool is_application(obj exp);
obj operator(obj exp);
obj operands(obj exp);
bool no_operands(obj ops);
obj first_operand(obj ops);
obj rest_operands(obj ops);
bool is_cond(obj);
obj cond_to_if(obj);
obj make_procedure(obj parameters, obj body, obj env);
bool is_compound_procedure(obj);
obj procedure_parameters(obj);
obj procedure_body(obj);
obj procedure_environment(obj p);
obj apply_primitive_procedure(obj proc, obj args);

bool is_let(obj);
obj let_to_combination(obj);
bool is_letstar(obj);
obj letstar_to_combination(obj);
bool is_letrec(obj);
obj letrec_to_combination(obj);
bool is_and(obj);
obj and_to_if(obj);
bool is_or(obj);
obj or_to_if(obj);
bool is_time(obj);
obj timed_expr(obj);
bool is_uapply(obj);
obj apply_to_application(obj exp);
obj apply_operator(obj exp);
obj apply_operands(obj exp);

bool is_delay(obj exp);
obj delay_to_lambda(obj exp);
bool is_cons_stream(obj exp);
obj cons_stream_to_cons(obj exp);

#endif
