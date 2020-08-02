#include "parser.h"
#include "eval.h"
#include "output.h"

int main(void)
{
	write(eval(read()));
	newline();

	return 0;
}
