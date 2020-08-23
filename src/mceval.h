#ifndef MCEVAL_H
#define MCEVAL_H
#include "sicpstd.h"

#include "environment.h"
#include "obj.h"


// obj apply(obj procedure, obj arguments);
// obj list_of_values(obj exps, obj env);
// obj eval_if(obj exp, obj env);
// obj eval_sequence(obj exps, obj env);
// obj eval_definition(obj exp, obj env);
bool is_self_evaluating(obj exp);
bool is_variable(obj exp);
bool is_definition(obj exp);
bool is_lambda(obj exp);
obj lambda_parameters(obj exp);
obj lambda_body(obj exp);
// obj definition_variable(obj exp);
// obj definition_value(obj exp);
// obj make_lambda(obj, obj);
bool is_if(obj);
// obj if_predicate(obj);
// obj if_consequent(obj);
// obj if_alternate(obj);
// obj make_if(obj predicate, obj consequent, obj alternative);
// bool is_last_exp(obj seq);
// obj first_exp(obj seq);
// obj rest_exps(obj seq);
// obj sequence_to_exp(obj seq);
// obj make_begin(obj seq);
bool is_application(obj exp);
obj operator(obj exp);
obj operands(obj exp);
bool no_operands(obj ops);
obj first_operand(obj ops);
obj rest_operands(obj ops);
// bool is_cond(obj);
// obj cond_clauses(obj);
// bool is_cond_else_clause(obj);
// obj cond_predicate(obj);
// obj cond_actions(obj);
// obj cond_to_if(obj);
// obj expand_clauses(obj);
obj make_procedure(obj parameters, obj body, obj env);
// obj procedure_environment(obj p);
// obj apply_primitive_procedure(obj proc, obj args);

// // Used for displaying procs:
// bool is_compound_procedure(obj);
// obj procedure_parameters(obj);
// obj procedure_body(obj);



#endif
