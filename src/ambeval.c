#include "ambeval.h"

#define AREA "EVAL"

#include "aneval.h"
#include "eceval.h"
#include "environment.h"
#include "list.h"
#include "parser.h"
#include "storage.h"

static obj evalstr(char *e, obj env)
{
	return eceval(readp(openin_string(e)), env);
}

static void init(obj execution_environment)
{
	ambenv = extend_environment(emptylst, emptylst, execution_environment,
				    of_identifier("init_analyzer"));

	add_primprocs(ambenv);

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
		"        ((apply? exp) (analyze-apply exp))"
		"        ((application? exp) (analyze-application exp))"
		"        (else"
		"         (error \" Unknown expression type-- ANALYZE \" exp))))",
		ambenv);
	evalstr("(define (analyze-self-evaluating exp)"
		"  (lambda (env) exp))",
		ambenv);
	evalstr("(define (analyze-quoted exp)"
		"  (let ((qval (text-of-quotation exp)))"
		"    (lambda (env) qval)))",
		ambenv);
	evalstr("(define (analyze-variable exp)"
		"  (lambda (env) (lookup-variable-value exp  env)))",
		ambenv);

	evalstr("(define (analyze-assignment exp)"
		"  (let ((var (assignment-variable exp))"
		"        (vproc (analyze (assignment-value exp))))"
		"    (lambda (env)"
		"      (set-variable-value! var (vproc env) env)"
		"      'ok)))",
		ambenv);

	evalstr("(define (analyze-definition exp)"
		"  (let ((var (definition-variable exp))"
		"        (vproc (analyze (definition-value exp))))"
		"    (lambda (env)"
		"      (define-variable! var (vproc env) env)"
		"      'ok)))",
		ambenv);

	evalstr("(define (analyze-if exp)"
		"  (let ((pproc (analyze (if-predicate exp)))"
		"        (cproc (analyze (if-consequent exp)))"
		"        (aproc (analyze (if-alternative exp))))"
		"    (lambda (env)"
		"      (if (true? (pproc env))"
		"          (cproc env)"
		"          (aproc env)))))",
		ambenv);
	evalstr("(define (analyze-lambda exp)"
		"  (let ((vars (lambda-parameters exp))"
		"        (bproc (analyze-sequence (lambda-body exp))))"
		"    (lambda (env) (make-procedure vars bproc env))))",
		ambenv);
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
		ambenv);
	evalstr("(define (analyze-apply exp)"
		"  (let ((fproc (analyze (apply-operator exp)))"
		"        (argsproc (analyze (apply-operands exp))))"
		"    (lambda (env)"
		"      (execute-application (fproc env)"
		"                           (argsproc env)))))",
		ambenv);
	evalstr("(define (analyze-application exp)"
		"  (let ((fproc (analyze (operator exp)))"
		"        (aprocs (map analyze (operands exp))))"
		"    (lambda (env)"
		"      (execute-application (fproc env)"
		"                           (map (lambda (aproc) (aproc env))"
		"                                aprocs)))))",
		ambenv);
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
		ambenv);
	evalstr("(define (analyze-time exp)"
		"  (let ((proc (analyze (timed-expr exp))))"
		"    (lambda (env)"
		"      (time (proc env)))))",
		ambenv);
}

obj ambeval(obj exp, obj exenv)
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
		cons(list2(quote, analyzed), list1(list2(quote, exenv)));

	return eceval(analyze_execute, exenv);
}
