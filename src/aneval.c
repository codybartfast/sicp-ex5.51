#include "aneval.h"

#define AREA "ANEVAL"

#include "eceval.h"
#include "environment.h"
#include "error.h"
#include "inport.h"
#include "list.h"
#include "mceval.h"
#include "parser.h"
#include "primproc.h"
#include "register.h"
#include "storage.h"

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

static obj is_apply_p(obj args)
{
	return is_anapply(car(args)) ? true_o : false_o;
}

static obj apply_operands_p(obj args)
{
	return apply_operands(car(args));
}

static obj apply_operator_p(obj args)
{
	return apply_operator(car(args));
}

static obj apply_primitive_procedure_p(obj args)
{
	return apply_primitive_procedure(car(args), cadr(args));
}

static obj is_assignment_p(obj args)
{
	return is_assignment(car(args)) ? true_o : false_o;
}

static obj assignment_value_p(obj args)
{
	return assignment_value(car(args));
}

static obj assignment_variable_p(obj args)
{
	return assignment_variable(car(args));
}

static obj is_begin_p(obj args)
{
	return is_begin(car(args)) ? true_o : false_o;
}

static obj begin_actions_p(obj args)
{
	return begin_actions(car(args));
}

static obj is_compound_procedure_p(obj args)
{
	return is_compound_procedure(car(args)) ? true_o : false_o;
}

static obj is_cond_p(obj args)
{
	return is_cond(car(args)) ? true_o : false_o;
}

static obj cond_to_if_p(obj args)
{
	return cond_to_if(car(args));
}

static obj is_cons_stream_p(obj args)
{
	return is_cons_stream(car(args)) ? true_o : false_o;
}

static obj cons_stream_to_cons_p(obj args)
{
	return cons_stream_to_cons(car(args));
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

static obj is_delay_p(obj args)
{
	return is_delay(car(args)) ? true_o : false_o;
}

static obj delay_to_lambda_p(obj args)
{
	return delay_to_lambda(car(args));
}

static obj extend_environment_p(obj args)
{
	return extend_environment(car(args), cadr(args), caddr(args),
				  cadddr(args));
}

static obj first_exp_p(obj args)
{
	return first_exp(car(args));
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

static obj is_lambda_p(obj args)
{
	return is_lambda(car(args)) ? true_o : false_o;
}

static obj lambda_parameters_p(obj args)
{
	return lambda_parameters(car(args));
}

static obj lambda_body_p(obj args)
{
	return lambda_body(car(args));
}

static obj is_last_exp_p(obj args)
{
	return is_last_exp(car(args)) ? true_o : false_o;
}

static obj is_let_p(obj args)
{
	return is_let(car(args)) ? true_o : false_o;
}

static obj let_to_combination_p(obj args)
{
	return let_to_combination(car(args));
}

static obj is_letrec_p(obj args)
{
	return is_letrec(car(args)) ? true_o : false_o;
}

static obj letrec_to_combination_p(obj args)
{
	return letrec_to_combination(car(args));
}

static obj is_letstar_p(obj args)
{
	return is_letstar(car(args)) ? true_o : false_o;
}

static obj letstar_to_combination_p(obj args)
{
	return letstar_to_combination(car(args));
}

static obj lookup_variable_value_p(obj args)
{
	return lookup_variable_value(car(args), cadr(args));
}

static obj make_procedure_p(obj args)
{
	return make_procedure(car(args), cadr(args), caddr(args));
}

static obj operator_p(obj args)
{
	return operator(car(args));
}

static obj operands_p(obj args)
{
	return operands(car(args));
}

static obj is_or_p(obj args)
{
	return is_or(car(args)) ? true_o : false_o;
}

static obj or_to_if_p(obj args)
{
	return or_to_if(car(args));
}

static obj is_primitive_procedure_p(obj args)
{
	return is_primitive_procedure(car(args)) ? true_o : false_o;
}

static obj procedure_body_p(obj args)
{
	return procedure_body(car(args));
}

static obj procedure_environment_p(obj args)
{
	return procedure_environment(car(args));
}

static obj procedure_parameters_p(obj args)
{
	return procedure_parameters(car(args));
}

static obj is_quoted_p(obj args)
{
	return is_quoted(car(args)) ? true_o : false_o;
}

static obj is_quasiquote_p(obj args)
{
	return is_quasiquote(car(args)) ? true_o : false_o;
}

static obj quasi_to_combination_p(obj args)
{
	return quasi_to_combination(car(args));
}

static obj rest_exps_p(obj args)
{
	return rest_exps(car(args));
}

static obj is_self_evaluating_p(obj args)
{
	return is_self_evaluating(car(args)) ? true_o : false_o;
}

static obj set_variable_value_p(obj args)
{
	return set_variable_value(car(args), cadr(args), caddr(args));
}

static obj text_of_quotation_p(obj args)
{
	return text_of_quotation(car(args));
}

static obj is_time_p(obj args)
{
	return is_time(car(args)) ? true_o : false_o;
}

static obj timed_expr_p(obj args)
{
	return timed_expr(car(args));
}

static obj is_true_p(obj args)
{
	return is_true(car(args)) ? true_o : false_o;
}

static obj is_variable_p(obj args)
{
	return is_variable(car(args)) ? true_o : false_o;
}

void add_primprocs(obj env)
{
	define_variable(of_identifier("and?"), of_function(is_and_p), env);
	define_variable(of_identifier("and->if"), of_function(and_to_if_p),
			env);
	define_variable(of_identifier("application?"),
			of_function(is_application_p), env);
	define_variable(of_identifier("apply?"), of_function(is_apply_p), env);
	define_variable(of_identifier("apply-operands"),
			of_function(apply_operands_p), env);
	define_variable(of_identifier("apply-operator"),
			of_function(apply_operator_p), env);
	define_variable(of_identifier("apply-primitive-procedure"),
			of_function(apply_primitive_procedure_p), env);
	define_variable(of_identifier("assignment?"),
			of_function(is_assignment_p), env);
	define_variable(of_identifier("assignment-value"),
			of_function(assignment_value_p), env);
	define_variable(of_identifier("assignment-variable"),
			of_function(assignment_variable_p), env);
	define_variable(of_identifier("car"), of_function(car_p), env);
	define_variable(of_identifier("cdr"), of_function(cdr_p), env);
	define_variable(of_identifier("cons-stream?"),
			of_function(is_cons_stream_p), env);
	define_variable(of_identifier("cons-stream->cons"),
			of_function(cons_stream_to_cons_p), env);
	define_variable(of_identifier("begin?"), of_function(is_begin_p), env);
	define_variable(of_identifier("begin-actions"),
			of_function(begin_actions_p), env);
	define_variable(of_identifier("compound-procedure?"),
			of_function(is_compound_procedure_p), env);
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
	define_variable(of_identifier("delay?"), of_function(is_delay_p), env);
	define_variable(of_identifier("delay->lambda"),
			of_function(delay_to_lambda_p), env);
	define_variable(of_identifier("first-exp"), of_function(first_exp_p),
			env);
	define_variable(of_identifier("extend-environment"),
			of_function(extend_environment_p), env);
	define_variable(of_identifier("if?"), of_function(is_if_p), env);
	define_variable(of_identifier("if-predicate"),
			of_function(if_predicate_p), env);
	define_variable(of_identifier("if-consequent"),
			of_function(if_consequent_p), env);
	define_variable(of_identifier("if-alternative"),
			of_function(if_alternate_p), env);
	define_variable(of_identifier("lambda?"), of_function(is_lambda_p),
			env);
	define_variable(of_identifier("lambda-parameters"),
			of_function(lambda_parameters_p), env);
	define_variable(of_identifier("lambda-body"),
			of_function(lambda_body_p), env);
	define_variable(of_identifier("last-exp?"), of_function(is_last_exp_p),
			env);
	define_variable(of_identifier("let?"), of_function(is_let_p), env);
	define_variable(of_identifier("let->combination"),
			of_function(let_to_combination_p), env);
	define_variable(of_identifier("letrec?"), of_function(is_letrec_p),
			env);
	define_variable(of_identifier("letrec->combination"),
			of_function(letrec_to_combination_p), env);
	define_variable(of_identifier("letstar?"), of_function(is_letstar_p),
			env);
	define_variable(of_identifier("letstar->combination"),
			of_function(letstar_to_combination_p), env);
	define_variable(of_identifier("lookup-variable-value"),
			of_function(lookup_variable_value_p), env);

	define_variable(of_identifier("make-procedure"),
			of_function(make_procedure_p), env);
	define_variable(of_identifier("nil"), emptylst, env);
	define_variable(of_identifier("null?"), of_function(is_null_p), env);
	define_variable(of_identifier("ok"), ok, env);
	define_variable(of_identifier("operator"), of_function(operator_p),
			env);
	define_variable(of_identifier("operands"), of_function(operands_p),
			env);
	define_variable(of_identifier("or?"), of_function(is_or_p), env);
	define_variable(of_identifier("or->if"), of_function(or_to_if_p), env);
	define_variable(of_identifier("primitive-procedure?"),
			of_function(is_primitive_procedure_p), env);
	define_variable(of_identifier("procedure-body"),
			of_function(procedure_body_p), env);
	define_variable(of_identifier("procedure-environment"),
			of_function(procedure_environment_p), env);
	define_variable(of_identifier("procedure-parameters"),
			of_function(procedure_parameters_p), env);
	define_variable(of_identifier("quoted?"), of_function(is_quoted_p),
			env);
	define_variable(of_identifier("quasiquote?"),
			of_function(is_quasiquote_p), env);
	define_variable(of_identifier("quasi->combination"),
			of_function(quasi_to_combination_p), env);
	define_variable(of_identifier("rest-exps"), of_function(rest_exps_p),
			env);
	define_variable(of_identifier("reverse"), of_function(reverse_p), env);
	define_variable(of_identifier("self-evaluating?"),
			of_function(is_self_evaluating_p), env);
	define_variable(of_identifier("set-variable-value!"),
			of_function(set_variable_value_p), env);
	define_variable(of_identifier("text-of-quotation"),
			of_function(text_of_quotation_p), env);
	define_variable(of_identifier("time?"), of_function(is_time_p), env);
	define_variable(of_identifier("timed-expr"), of_function(timed_expr_p),
			env);
	define_variable(of_identifier("true?"), of_function(is_true_p), env);
	define_variable(of_identifier("variable?"), of_function(is_variable_p),
			env);
}

static obj evalstr(char *e, obj env)
{
	return eceval(readp(openin_string(e)), env);
}

static void init(obj execution_environment)
{
	anenv = extend_environment(emptylst, emptylst, execution_environment,
				   of_identifier("init_analyzer"));

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
		"        ((quasiquote? exp) (analyze (quasi->combination exp)))"
		"        ((variable? exp) (analyze-variable exp))"
		"        ((assignment? exp) (analyze-assignment exp))"
		"        ((definition? exp) (analyze-definition exp))"
		"        ((if? exp) (analyze-if exp))"
		"        ((lambda? exp) (analyze-lambda exp))"
		"        ((begin? exp) (analyze-sequence (begin-actions exp)))"
		"        ((cond? exp) (analyze (cond->if exp)))"
		"        ((let? exp) (analyze (let->combination exp)))"
		"        ((letrec? exp) (analyze (letrec->combination exp)))"
		"        ((letstar? exp) (analyze (letstar->combination exp)))"
		"        ((and? exp) (analyze (and->if exp)))"
		"        ((or? exp) (analyze (or->if exp)))"
		"        ((delay? exp) (analyze (delay->lambda exp)))"
		"        ((cons-stream? exp) (analyze (cons-stream->cons exp)))"
		"        ((time? exp) (analyze-time exp))"
		"        ((apply? exp) (analyze-apply exp))"
		"        ((application? exp) (analyze-application exp))"
		"        (else"
		"         (error \" Unknown expression type-- ANALYZE \" exp))))",
		anenv);
	evalstr("(define (analyze-self-evaluating exp)"
		"  (lambda (env) exp))",
		anenv);
	evalstr("(define (analyze-quoted exp)"
		"  (let ((qval (text-of-quotation exp)))"
		"    (lambda (env) qval)))",
		anenv);
	evalstr("(define (analyze-variable exp)"
		"  (lambda (env) (lookup-variable-value exp  env)))",
		anenv);

	evalstr("(define (analyze-assignment exp)"
		"  (let ((var (assignment-variable exp))"
		"        (vproc (analyze (assignment-value exp))))"
		"    (lambda (env)"
		"      (set-variable-value! var (vproc env) env)"
		"      'ok)))",
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
	evalstr("(define (analyze-lambda exp)"
		"  (let ((vars (lambda-parameters exp))"
		"        (bproc (analyze-sequence (lambda-body exp))))"
		"    (lambda (env) (make-procedure vars bproc env))))",
		anenv);
	evalstr("(define (analyze-sequence exps)"
		"  (define (sequentially proc1 proc2)"
		"    (lambda (env) (proc1 env) (proc2 env)))"
		"  (define (loop first-proc rest-procs)"
		"    (if (null? rest-procs)"
		"        first-proc"
		"        (loop (sequentially first-proc (car rest-procs))"
		"              (cdr rest-procs))))"
		"  (let ((procs (map analyze exps)))"
		"    (if (null? procs)"
		"        (error \"Empty sequence -- ANALYZE\"))"
		"    (loop (car procs) (cdr procs))))",
		anenv);
	evalstr("(define (analyze-apply exp)"
		"  (let ((fproc (analyze (apply-operator exp)))"
		"        (argsproc (analyze (apply-operands exp))))"
		"    (lambda (env)"
		"      (execute-application (fproc env)"
		"                           (argsproc env)))))",
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
		"                              (procedure-environment proc)"
		"                              \"doh! analysis\")))"
		"        (else"
		"         (error"
		"          \"Unknown procedure type -- EXECUTE-APPLICATION\""
		"          proc))))",
		anenv);
	evalstr("(define (analyze-time exp)"
		"  (let ((proc (analyze (timed-expr exp))))"
		"    (lambda (env)"
		"      (time (proc env)))))",
		anenv);
}

obj aneval(obj exp, obj exenv)
{
	static bool haveinit = false;

	bool orig_gc = disable_gc;
	disable_gc = true;

	if (!haveinit) {
		init(exenv);
		haveinit = true;
	}

	obj analyzed =
		eceval(cons(of_identifier("analyze"), list1(list2(quote, exp))),
		       anenv);

	disable_gc = orig_gc;

	obj analyze_execute =
		cons(list2(quote, analyzed), list1(list2(quote, exenv)));

	return eceval(analyze_execute, exenv);
}
