#include <stdio.h>
#include "lexer.h"

void parse(struct in_port *);

int main(void)
{
	struct in_port *in_port;
	struct token *t;

	in_port = open_input_string("  123    456 7890 987654321");
	
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);

	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);

	close_input_port(in_port);

	return 0;
}
