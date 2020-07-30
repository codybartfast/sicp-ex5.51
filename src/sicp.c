#include <stdio.h>
#include "lexer.h"

void parse(struct in_port *);

int main(void)
{
	struct in_port *in_port;

	in_port = open_input_string("  123 456 7890");
	printf("'%s'\n", read_token(in_port)->value);
	printf("'%s'\n", read_token(in_port)->value);
	printf("'%s'\n", read_token(in_port)->value);
	close_input_port(in_port);
	
	return 0;
}
