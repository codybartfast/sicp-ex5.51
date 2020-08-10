#include "error.h"
#include "list.h"
#include "output.h"
#include "primproc.h"

#include "environment.h"

#define AREA "ENVIRONMENT"

static obj the_empty_environment(void);
static obj setup_environment(void);

// ln 233

static obj the_empty_environment(void)
{
	return Obj.empty();
}

// ln 235

static obj make_frame(obj variables, obj values)
{
	cons(variables, values);
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
	}
}

// ln 295

static obj _primitive_procedures;
static obj primitive_procedures(void)
{
	return ispair(_primitive_procedures) ?
		       _primitive_procedures :
		       (_primitive_procedures =
				list(list(Obj.ofidentifier("+"), Obj.offunction(add_pp)),
				     list(Obj.ofidentifier("-"), Obj.offunction(sub_pp))));
}

// ln 301

static obj primitive_procedure_names(void)
{
	return map_u(car, primitive_procedures());
}

// ln 305

static obj primitive_procedure_objects(void)
{
	return map_u(cdr, primitive_procedures());
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
	return ispair(_the_global_environment) ?
		       _the_global_environment :
		       (_the_global_environment = setup_environment());
}
