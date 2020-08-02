#include "parser.h"
#include "eval.h"
#include "convert.h"

#include "out_port.h"
#include "output.h"

int main(void)
{
	struct in_port *in = open_input_file_pointer(stdin);
	struct out_port *out = open_output_file_pointer(stdout);

	printf("> ");
	write(out, eval(read(in)));
	newline(out);

	return 0;
}
