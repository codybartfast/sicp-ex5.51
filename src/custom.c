#include "custom.h"

#include <ctype.h>
#include <stdlib.h>
#include "bitmap.h"
#include "environment.h"
#include "error.h"
#include "eval.h"
#include "list.h"
#include "load.h"
#include "parser.h"
#include "pict.h"
#include "primproc.h"
#include "storage.h"

#define AREA "CUSTOM"

static obj evalstr(char *e, obj env)
{
	return eval(readp(openin_string(e)), env);
}

static void add_accessors(obj env)
{
	define_variable(of_identifier("caar"), of_function(caar_p), env);
	define_variable(of_identifier("cadr"), of_function(cadr_p), env);
	define_variable(of_identifier("cdar"), of_function(cdar_p), env);
	define_variable(of_identifier("cddr"), of_function(cddr_p), env);
	define_variable(of_identifier("caaar"), of_function(caaar_p), env);
	define_variable(of_identifier("caadr"), of_function(caadr_p), env);
	define_variable(of_identifier("cadar"), of_function(cadar_p), env);
	define_variable(of_identifier("caddr"), of_function(caddr_p), env);
	define_variable(of_identifier("cdaar"), of_function(cdaar_p), env);
	define_variable(of_identifier("cdadr"), of_function(cdadr_p), env);
	define_variable(of_identifier("cddar"), of_function(cddar_p), env);
	define_variable(of_identifier("cdddr"), of_function(cdddr_p), env);
	define_variable(of_identifier("caaaar"), of_function(caaaar_p), env);
	define_variable(of_identifier("caaadr"), of_function(caaadr_p), env);
	define_variable(of_identifier("caadar"), of_function(caadar_p), env);
	define_variable(of_identifier("caaddr"), of_function(caaddr_p), env);
	define_variable(of_identifier("cadaar"), of_function(cadaar_p), env);
	define_variable(of_identifier("cadadr"), of_function(cadadr_p), env);
	define_variable(of_identifier("caddar"), of_function(caddar_p), env);
	define_variable(of_identifier("cadddr"), of_function(cadddr_p), env);
	define_variable(of_identifier("cdaaar"), of_function(cdaaar_p), env);
	define_variable(of_identifier("cdaadr"), of_function(cdaadr_p), env);
	define_variable(of_identifier("cdadar"), of_function(cdadar_p), env);
	define_variable(of_identifier("cdaddr"), of_function(cdaddr_p), env);
	define_variable(of_identifier("cddaar"), of_function(cddaar_p), env);
	define_variable(of_identifier("cddadr"), of_function(cddadr_p), env);
	define_variable(of_identifier("cdddar"), of_function(cdddar_p), env);
	define_variable(of_identifier("cddddr"), of_function(cddddr_p), env);
}

static void add_pict(obj env)
{
	evalstr("(define %pict-path \"pict\")", env);
	define_variable(of_identifier("__%%paint"), of_function(paint), env);

	define_variable(of_identifier("__%%hamilton"), hamiltonbmp, env);
	define_variable(of_identifier("__%%pattern"), patternbmp, env);
	define_variable(of_identifier("__%%rogers"), rogersbmp, env);
	define_variable(of_identifier("__%%sussman"), sussmanbmp, env);
	define_variable(of_identifier("draw-line"), of_function(draw_line),
			env);
	define_variable(of_identifier("write-frame"), of_function(write_canvas),
			env);

	evalstr("(define (make-vect xcor ycor)"
		"  (cons xcor ycor))",
		env);

	evalstr("(define (xcor-vect vect)"
		"  (car vect))",
		env);

	evalstr("(define (ycor-vect vect)"
		"  (cdr vect))",
		env);

	evalstr("(define (add-vect vect1 vect2)"
		"  (cons"
		"   (+ (xcor-vect vect1) (xcor-vect vect2))"
		"   (+ (ycor-vect vect1) (ycor-vect vect2))))",
		env);

	evalstr("(define (sub-vect vect1 vect2)"
		"  (cons"
		"   (- (xcor-vect vect1) (xcor-vect vect2))"
		"   (- (ycor-vect vect1) (ycor-vect vect2))))",
		env);

	evalstr("(define (scale-vect s vect)"
		"  (cons"
		"   (* s (xcor-vect vect))"
		"   (* s (ycor-vect vect))))",
		env);

	evalstr("(define (make-frame origin edge1 edge2)"
		"  (cons origin (cons edge1 edge2)))",
		env);

	evalstr("(define (make-frame origin edge1 edge2)"
		"  (cons origin (cons edge1 edge2)))",
		env);

	evalstr("(define (origin-frame frame)"
		"  (car frame))",
		env);

	evalstr("(define (edge1-frame frame)"
		"  (cadr frame))",
		env);

	evalstr("(define (edge2-frame frame)"
		"  (cddr frame))",
		env);

	evalstr("(define frame (make-frame (make-vect 0 0)"
		"                               (make-vect 1 0)"
		"                               (make-vect 0 1)))",
		env);

	evalstr("(define (hamilton  frame)"
		"  (__%%paint __%%hamilton frame))",
		env);
	evalstr("(define (pattern frame)"
		"  (__%%paint __%%pattern frame))",
		env);
	evalstr("(define (rogers frame)"
		"  (__%%paint __%%rogers frame))",
		env);
	evalstr("(define (sussman frame)"
		"  (__%%paint __%%sussman frame))",
		env);

	evalstr("(define (frame-coord-map frame)"
		"  (lambda (v)"
		"    (add-vect"
		"     (origin-frame frame)"
		"     (add-vect (scale-vect (xcor-vect v)"
		"                           (edge1-frame frame))"
		"               (scale-vect (ycor-vect v)"
		"                           (edge2-frame frame))))))",
		env);

	evalstr("(define (transform-painter painter origin corner1 corner2)"
		"  (lambda (frame)"
		"    (let ((m (frame-coord-map frame)))"
		"      (let ((new-origin (m origin)))"
		"        (painter"
		"         (make-frame new-origin"
		"                     (sub-vect (m corner1) new-origin)"
		"                     (sub-vect (m corner2) new-origin)))))))",
		env);

	evalstr("(define (flip-vert painter)"
		"  (transform-painter painter"
		"                     (make-vect 0 1)"
		"                     (make-vect 1 1)"
		"                     (make-vect 0 0)))",
		env);

	evalstr("(define (flip-horiz painter)"
		"  (transform-painter painter"
		"                     (make-vect 1 0)"
		"                     (make-vect 0 0)"
		"                     (make-vect 1 1)))",
		env);

	evalstr("(define (beside painter1 painter2)"
		"  (let ((split-point (make-vect 0.5 0)))"
		"    (let ((paint-left"
		"           (transform-painter painter1"
		"                              (make-vect 0 0)"
		"                              split-point"
		"                              (make-vect 0 1)))"
		"          (paint-right"
		"           (transform-painter painter2"
		"                              split-point"
		"                              (make-vect 1 0)"
		"                              (make-vect 0.5 1))))"
		"      (lambda (frame)"
		"        (paint-left frame)"
		"        (paint-right frame)))))",
		env);

	evalstr("(define (below painter1 painter2)"
		"  (let ((split-point (make-vect 0 0.5)))"
		"    (let ((paint-bottom"
		"           (transform-painter painter1"
		"                              (make-vect 0 0)"
		"                              (make-vect 1 0)"
		"                              split-point))"
		"          (paint-top"
		"           (transform-painter painter2"
		"                              split-point"
		"                              (make-vect 1 0.5)"
		"                              (make-vect 0 1))))"
		"      (lambda (frame)"
		"        (paint-bottom frame)"
		"        (paint-top frame)))))",
		env);

	evalstr("(define (right-split painter n)"
		"  (if (= n 0)"
		"      painter"
		"      (let ((smaller (right-split painter (- n 1))))"
		"        (beside painter (below smaller smaller)))))",
		env);

	evalstr("(define (up-split painter n)"
		"  (if (= n 0)"
		"      painter"
		"      (let ((smaller (up-split painter (- n 1))))"
		"        (below painter (beside smaller smaller)))))",
		env);

	evalstr("(define (make-segment start end)"
		"  (cons start end))",
		env);

	evalstr("(define (start-segment segment)"
		"  (car segment))",
		env);

	evalstr("(define (end-segment segment)"
		"  (cdr segment))",
		env);

	evalstr("(define (segments->painter segment-list)"
		"  (lambda (frame)"
		"    (for-each"
		"     (lambda (segment)"
		"       (draw-line"
		"        ((frame-coord-map frame) (start-segment segment))"
		"        ((frame-coord-map frame) (end-segment segment))))"
		"     segment-list)))",
		env);

	evalstr("(define wave"
		"  (segments->painter"
		"   (list"
		"    (make-segment (make-vect 0 0.839) (make-vect 0.155 0.601))"
		"    (make-segment (make-vect 0.155 0.601) (make-vect 0.301 0.653))"
		"    (make-segment (make-vect 0.301 0.653) (make-vect 0.404 0.653))"
		"    (make-segment (make-vect 0.404 0.653) (make-vect 0.351 0.85))"
		"    (make-segment (make-vect 0.351 0.85) (make-vect 0.393 1))"
		""
		"    (make-segment (make-vect 0.597 1) (make-vect 0.653 0.85))"
		"    (make-segment (make-vect 0.653 0.85) (make-vect 0.597 0.653))"
		"    (make-segment (make-vect 0.597 0.653) (make-vect 0.751 0.653))"
		"    (make-segment (make-vect 0.751 0.653) (make-vect 1 0.352))"
		""
		"    (make-segment (make-vect 1 0.155) (make-vect 0.601 0.456))"
		"    (make-segment (make-vect 0.601 0.456) (make-vect 0.751 0))"
		""
		"    (make-segment (make-vect 0.597 0) (make-vect 0.477 0.301))"
		"    (make-segment (make-vect 0.477 0.301) (make-vect 0.394 0))"
		""
		"    (make-segment (make-vect 0.252 0) (make-vect 0.352 0.508))"
		"    (make-segment (make-vect 0.352 0.508) (make-vect 0.301 0.596))"
		"    (make-segment (make-vect 0.301 0.596) (make-vect 0.155 0.399))"
		"    (make-segment (make-vect 0.155 0.399) (make-vect 0 0.642)))))",
		env);

	evalstr("(define painter"
		"  (let ((n (random 5)))"
		"    (cond"
		"      ((= n 0) hamilton)"
		"      ((= n 1) pattern)"
		"      ((= n 2) sussman)"
		"      ((= n 3) wave)"
		"      (else rogers))))",
		env);
}

static void add_optable(obj env)
{
	define_variable(of_identifier("__%%setd"), of_function(set_cdr_p), env);
	evalstr("(define (lookup key table)"
		"  (let ((record (assoc key (cdr table))))"
		"    (if record"
		"        (cdr record)"
		"        false)))",
		env);
	evalstr("(define (assoc key records)"
		"  (cond ((null? records) false)"
		"        ((equal? key (caar records)) (car records))"
		"        (else (assoc key (cdr records)))))",
		env);
	evalstr("(define (make-table)"
		"  (let ((local-table (list '*table*)))"
		"    (define (lookup key-1 key-2)"
		"      (let ((subtable (assoc key-1 (cdr local-table))))"
		"        (if subtable"
		"            (let ((record (assoc key-2 (cdr subtable))))"
		"              (if record"
		"                  (cdr record)"
		"                  false))"
		"            false)))"
		"    (define (insert! key-1 key-2 value)"
		"      (let ((subtable (assoc key-1 (cdr local-table))))"
		"        (if subtable"
		"            (let ((record (assoc key-2 (cdr subtable))))"
		"              (if record"
		"                  (__%%setd record value)"
		"                  (__%%setd subtable"
		"                            (cons (cons key-2 value)"
		"                                  (cdr subtable)))))"
		"            (__%%setd local-table"
		"                      (cons (list key-1"
		"                                  (cons key-2 value))"
		"                            (cdr local-table)))))"
		"      'ok)"
		"    (define (dispatch m)"
		"      (cond ((eq? m 'lookup-proc) lookup)"
		"            ((eq? m 'insert-proc!) insert!)"
		"            (else (error \"Unknown operation-- TABLE\" m))))"
		"    dispatch))",
		env);
	evalstr("(define operation-table (make-table))", env);
	evalstr("(define get (operation-table 'lookup-proc))", env);
	evalstr("(define put (operation-table 'insert-proc!))", env);
}

static obj add_extras(int ex, obj env)
{
	define_variable(of_identifier("%ex"), of_function(pcnt_ex), env);
	define_variable(of_identifier("load"), of_function(loadq), env);
	define_variable(of_identifier("loadv"), of_function(loadv), env);

	if (ex > 101) {
		//define_variable(of_identifier("abs"), of_function(absl), env);
		evalstr("(define (abs x) (if (< x 0) (- x) x))", env);

		define_variable(of_identifier("<="), of_function(lte), env);
		//evalstr("(define (<= x y) (not (> x y)))", env);

		define_variable(of_identifier(">="), of_function(gte), env);
		//evalstr("(define (>= x y) (not (< x y)))", env);

		evalstr("(define (square x) (* x x))", env);
		evalstr("(define (cube x) (* x x x))", env);
	}
	if (ex >= 109) {
		define_variable(of_identifier("exp"), of_function(expn), env);
		define_variable(of_identifier("log"), of_function(logn), env);
		// inc and dec could be 'defined' but making them primitive
		// avoids the risk of loops in questions where + is defined in
		// terms of inc
		define_variable(of_identifier("inc"), of_function(inc), env);
		define_variable(of_identifier("dec"), of_function(dec), env);
	}
	if (ex >= 106) {
		evalstr("(define (average x y) (/ (+ x y) 2))", env);
	}
	if (ex >= 115) {
		define_variable(of_identifier("remainder"), of_function(rem),
				env);
		evalstr("(define (quotient a b)"
			"  (/ (- a (remainder a b))"
			"     b))",
			env);
	}
	if (ex >= 116) {
		evalstr("(define (even? n) (= (remainder n 2) 0))", env);
		evalstr("(define (odd? n) (= (remainder n 2) 1))", env);
	}
	if (ex >= 120) {
		evalstr("(define (gcd a b) (if (= b 0) a (gcd b (remainder a b))))",
			env);
		define_variable(of_identifier("random"), of_function(rnd), env);
	}
	if (ex >= 121) {
		evalstr("(define (prime? n)"
			"  (define (smallest-divisor n)"
			"    (define (find-divisor n test-divisor)"
			"      (define (divides? a b)"
			"        (= (remainder b a) 0))"
			"      (cond ((> (square test-divisor) n) n)"
			"            ((divides? test-divisor n) test-divisor)"
			"            (else (find-divisor n (+ test-divisor 1)))))"
			"    (find-divisor n 2))"
			"  (= n (smallest-divisor n)))",
			env);
	}
	if (ex >= 122) {
		define_variable(of_identifier("display"), of_function(display),
				env);
		define_variable(of_identifier("ignore"), of_function(ignore),
				env);
		define_variable(of_identifier("newline"), of_function(newline),
				env);
		define_variable(of_identifier("runtime"), of_function(runtime),
				env);
		define_variable(of_identifier("seconds"), of_function(seconds),
				env);
		define_variable(of_identifier("ticks"), of_function(ticks),
				env);
	}
	if (ex >= 129) {
		evalstr("(define (identity x) x)", env);
	}
	if (ex >= 135) {
		define_variable(of_identifier("sin"), of_function(sine), env);
		define_variable(of_identifier("cos"), of_function(cosine), env);
		define_variable(of_identifier("error"), of_function(user_error),
				env);
		evalstr("(define (positive? x) (< 0 x))", env);
		evalstr("(define (negative? x) (< x 0))", env);
	}
	if (ex >= 145) {
		// not in the book but most answers use it.
		define_variable(of_identifier("floor"), of_function(flr), env);
	}
	if (ex >= 201) {
		define_variable(of_identifier("cons"), of_function(cons_p),
				env);
		define_variable(of_identifier("car"), of_function(car_p), env);
		define_variable(of_identifier("cdr"), of_function(cdr_p), env);
	}
	if (ex >= 203) {
		define_variable(of_identifier("sqrt"), of_function(sqroot),
				env);
	}
	if (ex >= 205) {
		evalstr("(define (expt b n)"
			"  (define (expt-iter b c p)"
			"    (if (= c 0) p (expt-iter b (- c 1) (* b p))))"
			"  (expt-iter b n 1))",
			env);
	}
	if (ex >= 207) {
		define_variable(of_identifier("min"), of_function(minimum),
				env);
		define_variable(of_identifier("max"), of_function(maximum),
				env);
	}
	if (ex >= 217) {
		define_variable(of_identifier("nil"), emptylst, env);
		define_variable(of_identifier("list"), of_function(list), env);
		evalstr("(define (list-ref items n)"
			"  (if (= n 0)"
			"      (car items)"
			"      (list-ref (cdr items) (- n 1))))",
			env);
		define_variable(of_identifier("null?"), of_function(is_null_p),
				env);
		define_variable(of_identifier("length"), of_function(length_p),
				env);
		define_variable(of_identifier("reverse"),
				of_function(reverse_p), env);
		define_variable(of_identifier("append"), of_function(append_p),
				env);
		add_accessors(env);
	}
	if (ex >= 221) {
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
			"              (cons (apply proc (smap car arglists))"
			"                    mapped))))"
			"  (reverse (iter arglists nil)))",
			env);
	}
	if (ex >= 224) {
		define_variable(of_identifier("pair?"), of_function(is_pair_p),
				env);
		evalstr("(define (for-each proc items)"
			"  (cond ((not (null? items))"
			"          (proc (car items))"
			"          (for-each proc (cdr items)))))",
			env);
	}
	if (ex >= 244) {
		add_pict(env);
	}
	if (ex >= 253) {
		define_variable(of_identifier("eq?"), of_function(is_eq_p),
				env);
		evalstr("(define (memq item x)"
			"  (cond ((null? x) false)"
			"        ((eq? item (car x)) x)"
			"        (else (memq item (cdr x)))))",
			env);
	}
	if (ex >= 254) {
		define_variable(of_identifier("equal?"),
				of_function(is_equal_p), env);
	}
	if (ex >= 256) {
		define_variable(of_identifier("number?"),
				of_function(is_number_p), env);
		define_variable(of_identifier("symbol?"),
				of_function(is_symbol_p), env);
	}
	if (ex >= 273) {
		add_optable(env);
		define_variable(of_identifier("atan"), of_function(arctan),
				env);
		evalstr("(define pi 3.14159265358979323846)", env);
	}
	return unspecified;
}

static int ex_num(obj arg)
{
	const char *str;
	int ok = false;
	if (is_string(arg)) {
		str = to_string(arg);
		if (isdigit(str[0]) && (str[1] == '.') && isdigit(str[2]) &&
		    (str[3] == '\0' || (isdigit(str[3]) && str[4] == '\0'))) {
			ok = true;
		}
	}
	if (!ok) {
		return -1;
	}
	return (100 * (str[0] - 48)) + atoi(str + 2);
}

static obj conf_ex(obj env, obj args)
{
	obj arg;
	int ex;
	if (is_err(arg = chkarity("%ex", 1, args)))
		return arg;
	if ((ex = ex_num(car(args))) == -1)
		return error_argument_type(
			AREA, "%%ex expects a string of from \"1.23\"");
	return add_extras(ex, env);
}

obj do_head(obj env, struct inport *in)
{
	disable_gc = true;
	obj exp = readp(in);
	if (is_pair(exp) && is_eq(car(exp), _ex)) {
		obj r = conf_ex(env, cdr(exp));
		if (is_err(r))
			return r;
		exp = readp(in);
	} else {
		add_extras(552, env);
	}
	disable_gc = false;
	return exp;
}

static obj defined(void)
{
	return caar(the_global_environment());
}

static void display_id(struct outport *out, obj id)
{
	obj lpad = of_string("  ");

	newlinep(out);
	displayp(out, lpad);
	displayp(out, id);
}

static obj display_definedp(struct outport *out)
{
	obj names;

	newlinep(out);
	displayp(out, of_string("Special Forms:"));
	display_id(out, of_string("and"));
	display_id(out, of_string("apply"));
	display_id(out, of_string("begin"));
	display_id(out, of_string("cond"));
	display_id(out, of_string("define"));
	display_id(out, of_string("if"));
	display_id(out, of_string("lambda"));
	display_id(out, of_string("let"));
	display_id(out, of_string("or"));
	display_id(out, of_string("quote"));
	display_id(out, of_string("set!"));
	display_id(out, of_string("time"));

	newlinep(out);
	newlinep(out);
	displayp(out, of_string("Defined Variables:"));
	for (names = reverse(defined()); is_pair(names); names = cdr(names)) {
		display_id(out, car(names));
	}
	newlinep(out);
	return _void;
}

obj display_defined(obj _)
{
	(void)_;
	return display_definedp(default_out());
}
