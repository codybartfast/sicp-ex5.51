#include "parser.h"
#include "eval.h"
#include "convert.h"

#include "outport.h"
#include "output.h"


int main(void)
{
	printf("> ");
	write(eval(read()));
	newline();

	return 0;
}
