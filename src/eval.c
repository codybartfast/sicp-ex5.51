#include "eval.h"

#include "eceval.h"
#include "ambeval.h"
#include "aneval.h"

obj (*eval)(obj, obj) = eceval;

void use_ambeval(void)
{
	eval = ambeval2;
}

void use_aneval(void)
{
	eval = aneval;
}
