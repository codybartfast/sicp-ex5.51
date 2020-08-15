#include "environment.h"

#include <string.h>
#include "error.h"
#include "list.h"
#include "output.h"
#include "primproc.h"

#define AREA "ENVIRONMENT"

static obj the_empty_environment(void);
static obj lvv_env_loop(obj var, obj env);
static obj setup_environment(void);

// until we have a proper eq?
static bool eq_symbol(obj a, obj b)
{
	return is_symbol(a) && is_symbol(b) &&
	       strcmp(to_string(a), to_string(b)) == 0;
}

//231
static obj enclosing_environment(obj env)
{
	return cdr(env);
}

//232
static obj first_frame(obj env)
{
	return car(env);
}

// ln 233
static obj the_empty_environment(void)
{
	return emptylst;
}

// ln 235
static obj make_frame(obj variables, obj values)
{
	return cons(variables, values);
}

// ln 237
static obj frame_variables(obj frame)
{
	return car(frame);
}

// ln 238
static obj frame_values(obj frame)
{
	return cdr(frame);
}

// ln 243
static obj extend_environment(obj vars, obj vals, obj base_env)
{
	if (length_u(vars) == length_u(vals)) {
		return cons(make_frame(vars, vals), base_env);
	} else {
		eprintf(AREA, "Too %s arguments supplied",
			length_u(vars) < length_u(vals) ? "many" : "few",
			writestr(vars), writestr(vars));
		return error_argument_type();
	}
}

// ln 250
static obj lvv_scan(obj var, obj env, obj vars, obj vals)
{
	if (is_null(vars)) {
		return lvv_env_loop(var, enclosing_environment(env));
	} else if (eq_symbol(var, car(vars))) {
		return car(vals);
	} else {
		return lvv_scan(var, env, cdr(vars), cdr(vals));
	}
}

static obj lvv_env_loop(obj var, obj env)
{
	if (is_null(env)) { // should be eq - but don't have it yet.
		eprintf(AREA, "Unbound variable: %s", to_string(var));
		return error_unbound_variable();
	}
	obj frame = first_frame(env);
	return lvv_scan(var, env, frame_variables(frame), frame_values(frame));
}

obj lookup_variable_value(obj var, obj env)
{
	//printf("looking up: %s\n", debugstr(var));
	obj val = lvv_env_loop(var, env);
	//printf("looked up: %s\n", debugstr(val));
	return val;
}

// ln 295
static obj _primitive_procedures;
static obj primitive_procedures(void)
{
	return is_pair(_primitive_procedures) ?
		       _primitive_procedures :
		       (_primitive_procedures = listn(
				4,
				list2(of_identifier("+"), of_function(add_pp)),
				list2(of_identifier("-"), of_function(sub_pp)),
				list2(of_identifier("*"), of_function(mul_pp)),
				list2(of_identifier("/"),
				      of_function(div_pp))));
}

// ln 301
static obj primitive_procedure_names(void)
{
	return map_u(car, primitive_procedures());
}

// ln 305
static obj primitive_procedure_objects(void)
{
	return map_u(cadr, primitive_procedures());
}

// ln 309
static obj setup_environment(void)
{
	obj initial_env = extend_environment(primitive_procedure_names(),
					     primitive_procedure_objects(),
					     the_empty_environment());
	// define_variable(Obj.ofidentifier("true"), Obj.true);
	// define_variable(Obj.ofidentifier("false"), Obj.false);
	return initial_env;
}

// ln 317
obj _the_global_environment;
obj the_global_environment(void)
{
	return is_pair(_the_global_environment) ?
		       _the_global_environment :
		       (_the_global_environment = setup_environment());
}
