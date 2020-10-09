#include "eval.h"

#include "eceval.h"
#include "aneval.h"

obj (*eval)(obj, obj) = eceval;

void use_aneval(void){
	eval = aneval;
}

