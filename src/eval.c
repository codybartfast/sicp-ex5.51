#include "eval.h"

#include "eceval.h"

obj eval(obj exp, obj env){
	return eceval(exp, env);
}
