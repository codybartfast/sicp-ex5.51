#include "ambeval.h"

#define AREA "AMBEVAL"

#include "aneval.h"
#include "eceval.h"
#include "environment.h"
#include "error.h"
#include "list.h"
#include "mceval.h"
#include "parser.h"
#include "storage.h"

static obj is_amb_p(obj args)
{
	return is_amb(car(args)) ? true_o : false_o;
}

static obj amb_choices_p(obj args)
{
	return cdar(args);
}

static obj evalstr(char *e, obj env)
{
	return eceval(readp(openin_string(e)), env);
}

#include "output.h"

static void init(obj execution_environment)
{
	ambenv = extend_environment(emptylst, emptylst, execution_environment,
				    of_identifier("init_analyzer"));

	add_primprocs(ambenv);

	define_variable(of_identifier("amb?"), of_function(is_amb_p), ambenv);
	define_variable(of_identifier("amb-choices"),
			of_function(amb_choices_p), ambenv);

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
		ambenv);

	evalstr("(define (analyze exp)"
		"  (cond ((self-evaluating? exp) "
		"         (analyze-self-evaluating exp))"
		"        ((quoted? exp) (analyze-quoted exp))"
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
		"        ((amb? exp) (analyze-amb exp))"
		"        ((apply? exp) (analyze-apply exp))"
		"        ((application? exp) (analyze-application exp))"
		"        (else"
		"         (error \" Unknown expression type-- ANALYZE \" exp))))",
		ambenv);
	evalstr("(define (analyze-self-evaluating exp)"
		"  (lambda (env succeed fail)"
		"    (succeed exp fail)))",
		ambenv);
	evalstr("(define (analyze-quoted exp)"
		"  (let ((qval (text-of-quotation exp)))"
		"    (lambda (env succeed fail)"
		"      (succeed qval fail))))",
		ambenv);
	evalstr("(define (analyze-variable exp)"
		"  (lambda (env succeed fail)"
		"    (succeed (lookup-variable-value exp env)"
		"             fail)))",
		ambenv);
	evalstr("(define (analyze-lambda exp)"
		"  (let ((vars (lambda-parameters exp))"
		"        (bproc (analyze-sequence (lambda-body exp))))"
		"    (lambda (env succeed fail)"
		"      (succeed (make-procedure vars bproc env)"
		"fail))))",
		ambenv);
	evalstr("(define (analyze-if exp)"
		"  (let ((pproc (analyze (if-predicate exp)))"
		"        (cproc (analyze (if-consequent exp)))"
		"        (aproc (analyze (if-alternative exp))))"
		"    (lambda (env succeed fail)"
		"      (pproc env"
		"             (lambda (pred-value fail2)"
		"               (if (true? pred-value)"
		"                   (cproc env succeed fail2)"
		"                   (aproc env succeed fail2)))"
		"             fail))))",
		ambenv);
	evalstr("(define (analyze-sequence exps)"
		"  (define (sequentially a b)"
		"    (lambda (env succeed fail)"
		"      (a env"
		"         (lambda (a-value fail2)"
		"           (b env succeed fail2))"
		"         fail)))"
		"  (define (loop first-proc rest-procs)"
		"    (if (null? rest-procs)"
		"        first-proc"
		"        (loop (sequentially first-proc (car rest-procs))"
		"              (cdr rest-procs))))"
		"  (let ((procs (map analyze exps)))"
		"    (if (null? procs)"
		"        (error \"Empty sequence -- ANALYZE\"))"
		"    (loop (car procs) (cdr procs))))",
		ambenv);
	evalstr("(define (analyze-definition exp)"
		"  (let ((var (definition-variable exp))"
		"        (vproc (analyze (definition-value exp))))"
		"    (lambda (env succeed fail)"
		"      (vproc env"
		"             (lambda (val fail2)"
		"               (define-variable! var val env)"
		"               (succeed 'ok fail2))"
		"             fail))))",
		ambenv);
	evalstr("(define (analyze-assignment exp)"
		"  (let ((var (assignment-variable exp))"
		"        (vproc (analyze (assignment-value exp))))"
		"    (lambda (env succeed fail)"
		"      (vproc env"
		"             (lambda (val fail2)"
		"               (let ((old-value"
		"                      (lookup-variable-value var env)))"
		"                 (set-variable-value! var val env)"
		"                 (succeed 'ok"
		"                          (lambda ()"
		"                            (set-variable-value! var"
		"                                                 old-value"
		"                                                 env)"
		"                            (fail2)))))"
		"             fail))))",
		ambenv);
	evalstr("(define (analyze-apply exp)"
		"  (let ((fproc (analyze (apply-operator exp)))"
		"        (argsproc (analyze (apply-operands exp))))"
		"    (lambda (env succeed fail) "
		"      (fproc env"
		"             (lambda (proc fail2)"
		"              	(argsproc env"
		"              		(lambda (args fail3)"
		"              			(execute-application proc args succeed fail3))"
		"              		fail2))"
		"             fail))))",
		ambenv);
	evalstr("(define (analyze-application exp)"
		"  (let ((fproc (analyze (operator exp)))"
		"        (aprocs (map analyze (operands exp))))"
		"    (lambda (env succeed fail) "
		"      (fproc env"
		"             (lambda (proc fail2)"
		"               (get-args aprocs"
		"                         env"
		"                         (lambda (args fail3)"
		"                           (execute-application"
		"                            proc args succeed fail3))"
		"                         fail2))"
		"             fail))))",
		ambenv);
	evalstr("(define (get-args aprocs env succeed fail)"
		"  (if (null? aprocs)"
		"      (succeed '() fail)"
		"      ((car aprocs) env"
		"                    (lambda (arg fail2)"
		"                      (get-args (cdr aprocs)"
		"                                env"
		"                                (lambda (args fail3)"
		"                                  (succeed (cons arg args)"
		"                                           fail3))"
		"                                fail2))"
		"                    fail)))",
		ambenv);
	evalstr("(define (execute-application proc args succeed fail)"
		"  (cond ((primitive-procedure? proc)"
		"         (succeed (apply-primitive-procedure proc args)"
		"                  fail))"
		"        ((compound-procedure? proc)"
		"         ((procedure-body proc)"
		"          (extend-environment (procedure-parameters proc)"
		"                              args"
		"                              (procedure-environment proc)"
		"                              \"doh! amb analysis\")"
		"          succeed"
		"          fail))"
		"        (else"
		"         (error"
		"          \"Unknown procedure type -- EXECUTE-APPLICATION\""
		"          proc))))",
		ambenv);

	evalstr("(define (analyze-time exp)"
		"  (let ((proc (analyze (timed-expr exp))))"
		"    (lambda (env succeed fail)"
		"      (time (proc env succeed fail)))))",
		ambenv);
	evalstr("(define (analyze-amb exp)"
		"  (let ((cprocs (map analyze (amb-choices exp))))"
		"    (lambda (env succeed fail)"
		"      (define (try-next choices)"
		"        (if (null? choices)"
		"            (fail)"
		"            ((car choices) env"
		"                           succeed"
		"                           (lambda ()"
		"                             (try-next (cdr choices))))))"
		"      (try-next cprocs))))",
		ambenv);
}

static obj ambeval(obj exp, obj exenv, obj succeed, obj fail)
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
		       ambenv);

	disable_gc = orig_gc;

	obj analyze_execute =
		cons(list2(quote, analyzed),
		     list3(list2(quote, exenv), list2(quote, succeed),
			   list2(quote, fail)));

	return eceval(analyze_execute, exenv);
}

obj ambeval2(obj exp, obj exenv)
{
	obj value = of_identifier("value");
	obj fail_s = of_identifier("fail");
	obj succeed =
		make_procedure(list2(value, fail_s), list1(value), emptylst);
	obj fail = make_procedure(
		emptylst,
		list1( // list2(of_identifier("display"),
			//    of_string(
			//	    "ERROR: (AMBEVAL) unexpected call to amb2's fail")),
			list2(quote, amb_fail)),
		emptylst);
	return ambeval(exp, exenv, succeed, fail);
}
