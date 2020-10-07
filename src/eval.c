#include "eval.h"

#define AREA "EVAL"

#include "eceval.h"
#include "environment.h"
#include "error.h"
#include "inport.h"
#include "list.h"
#include "mceval.h"
#include "parser.h"
#include "primproc.h"
#include "storage.h"

// static obj anenv;

// Analyzing primitives

static obj is_and_p(obj args)
{
	return is_and(car(args)) ? true_o : false_o;
}

static obj and_to_if_p(obj args)
{
	return and_to_if(car(args));
}

static obj is_application_p(obj args)
{
	return is_application(car(args)) ? true_o : false_o;
}

static obj apply_primitive_procedure_p(obj args)
{
	return apply_primitive_procedure(car(args), cadr(args));
}

static obj is_cond_p(obj args)
{
	return is_cond(car(args)) ? true_o : false_o;
}

static obj cond_to_if_p(obj args)
{
	return cond_to_if(car(args));
}

static obj is_definition_p(obj args)
{
	return is_definition(car(args)) ? true_o : false_o;
}

static obj define_variable_p(obj args)
{
	return define_variable(car(args), cadr(args), caddr(args));
}

static obj definition_value_p(obj args)
{
	return definition_value(car(args));
}

static obj definition_variable_p(obj args)
{
	return definition_variable(car(args));
}

static obj is_if_p(obj args)
{
	return is_if(car(args)) ? true_o : false_o;
}

static obj if_predicate_p(obj args)
{
	return if_predicate(car(args));
}

static obj if_consequent_p(obj args)
{
	return if_consequent(car(args));
}

static obj if_alternate_p(obj args)
{
	return if_alternate(car(args));
}

static obj lookup_variable_value_p(obj args)
{
	return lookup_variable_value(car(args), cadr(args));
}

static obj is_quoted_p(obj args)
{
	return is_quoted(car(args)) ? true_o : false_o;
}

static obj operator_p(obj args)
{
	return operator(car(args));
}

static obj operands_p(obj args)
{
	return operands(car(args));
}

static obj is_primitive_procedure_p(obj args)
{
	return is_primitive_procedure(car(args)) ? true_o : false_o;
}

static obj is_self_evaluating_p(obj args)
{
	return is_self_evaluating(car(args)) ? true_o : false_o;
}

static obj is_true_p(obj args)
{
	return is_true(car(args)) ? true_o : false_o;
}

static obj is_variable_p(obj args)
{
	return is_variable(car(args)) ? true_o : false_o;
}

static void add_primprocs(obj env)
{
	define_variable(of_identifier("and?"), of_function(is_and_p), env);
	define_variable(of_identifier("and->if"), of_function(and_to_if_p),
			env);
	define_variable(of_identifier("application?"),
			of_function(is_application_p), env);
	define_variable(of_identifier("apply-primitive-procedure"),
			of_function(apply_primitive_procedure_p), env);
	define_variable(of_identifier("car"), of_function(car_p), env);
	define_variable(of_identifier("cdr"), of_function(cdr_p), env);
	define_variable(of_identifier("cons"), of_function(cons_p), env);
	define_variable(of_identifier("cond?"), of_function(is_cond_p), env);
	define_variable(of_identifier("cond->if"), of_function(cond_to_if_p),
			env);
	define_variable(of_identifier("definition?"),
			of_function(is_definition_p), env);
	define_variable(of_identifier("define-variable!"),
			of_function(define_variable_p), env);
	define_variable(of_identifier("definition-value"),
			of_function(definition_value_p), env);
	define_variable(of_identifier("definition-variable"),
			of_function(definition_variable_p), env);
	define_variable(of_identifier("if?"), of_function(is_if_p), env);
	define_variable(of_identifier("if-predicate"),
			of_function(if_predicate_p), env);
	define_variable(of_identifier("if-consequent"),
			of_function(if_consequent_p), env);
	define_variable(of_identifier("if-alternative"),
			of_function(if_alternate_p), env);
	define_variable(of_identifier("lookup-variable-value"),
			of_function(lookup_variable_value_p), env);

	define_variable(of_identifier("nil"), emptylst, env);
	define_variable(of_identifier("null?"), of_function(is_null_p), env);
	define_variable(of_identifier("ok"), ok, env);
	define_variable(of_identifier("operator"), of_function(operator_p),
			env);
	define_variable(of_identifier("operands"), of_function(operands_p),
			env);
	define_variable(of_identifier("primitive-procedure?"),
			of_function(is_primitive_procedure_p), env);
	define_variable(of_identifier("quoted?"), of_function(is_quoted_p),
			env);
	define_variable(of_identifier("reverse"), of_function(reverse_p), env);
	define_variable(of_identifier("self-evaluating?"),
			of_function(is_self_evaluating_p), env);
	define_variable(of_identifier("true?"), of_function(is_true_p), env);
	define_variable(of_identifier("variable?"), of_function(is_variable_p),
			env);
}

////////////////////////////////////

static obj evalstr(char *e, obj env)
{
	// printf("EVALSTR: %s\n", e);
	return eceval(readp(openin_string(e)), env);
}

#include <stdio.h>
obj eval(obj exp, obj exenv)
{
	disable_gc = true;
	obj anenv = setup_environment();
	add_primprocs(anenv);

	evalstr("(define (map proc . arglists)"
		"  (define (smap proc items)"
		"    (define (iter items mapped)"
		"      (if (null? items)"
		"          mapped"
		"          (iter (cdr items)"
		"                (cons (proc (car items))"
		"                      mapped))))"
		"    (reverse (iter items nil)))"
		"  (define (iter arglists mapped)"
		"    (if (null? (car arglists))"
		"        mapped"
		"        (iter (smap cdr arglists)"
		"              (cons (__%%apply proc (smap car arglists))"
		"                    mapped))))"
		"  (reverse (iter arglists nil)))",
		anenv);

	evalstr("(define (analyze exp)"
		"  (cond ((self-evaluating? exp) "
		"         (analyze-self-evaluating exp))"
		"        ((quoted? exp) (analyze-quoted exp))"
		"        ((variable? exp) (analyze-variable exp))"
		// "        ((assignment? exp) (analyze-assignment exp))"
		"        ((definition? exp) (analyze-definition exp))"
		"        ((if? exp) (analyze-if exp))"
		// "        ((lambda? exp) (analyze-lambda exp))"
		// "        ((begin? exp) (analyze-sequence (begin-actions exp)))"
		"        ((cond? exp) (analyze (cond->if exp)))"
		"        ((and? exp) (analyze (and->if exp)))"
		"        ((application? exp) (analyze-application exp))"
		"        (else"
		// "          exp)))",
		"         (error \" Unknown expression type-- ANALYZE \" exp))))",
		anenv);
	evalstr("(define (analyze-self-evaluating exp)"
		"  (lambda (env) exp))",
		anenv);
	evalstr("(define (analyze-variable exp)"
		"  (lambda (env) (lookup-variable-value exp  env)))",
		anenv);
	evalstr("(define (analyze-definition exp)"
		"  (let ((var (definition-variable exp))"
		"        (vproc (analyze (definition-value exp))))"
		"    (lambda (env)"
		"      (define-variable! var (vproc env) env)"
		"      'ok)))",
		anenv);

	evalstr("(define (analyze-if exp)"
		"  (let ((pproc (analyze (if-predicate exp)))"
		"        (cproc (analyze (if-consequent exp)))"
		"        (aproc (analyze (if-alternative exp))))"
		"    (lambda (env)"
		"      (if (true? (pproc env))"
		"          (cproc env)"
		"          (aproc env)))))",
		anenv);
	evalstr("(define (analyze-application exp)"
		"  (let ((fproc (analyze (operator exp)))"
		"        (aprocs (map analyze (operands exp))))"
		"    (lambda (env)"
		"      (execute-application (fproc env)"
		"                           (map (lambda (aproc) (aproc env))"
		"                                aprocs)))))",
		anenv);
	evalstr("(define (execute-application proc args)"
		"  (cond ((primitive-procedure? proc)"
		"         (apply-primitive-procedure proc args))"
		"        ((compound-procedure? proc)"
		"         ((procedure-body proc)"
		"          (extend-environment (procedure-parameters proc)"
		"                              args"
		"                              (procedure-environment proc))))"
		"        (else"
		"         (error"
		"          \"Unknown procedure type -- EXECUTE-APPLICATION\""
		"          proc))))",
		anenv);

	obj analyze_execute =
		cons(cons(of_identifier("analyze"), list1(list2(quote, exp))),
		     list1(list2(quote, exenv)));
	// printf("an exp: %s\n", errstr(exp));
	// printf("ex exp: %s\n", errstr(analyze_execute));
	disable_gc = false;
	return eceval(analyze_execute, anenv);
}
