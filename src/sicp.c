#include <stdio.h>
#include "lexer.h"

void parse(struct in_port *);

int main(void)
{
	struct in_port *in_port;
	struct token *t;

	in_port = open_input_string("  123    456 7890 9876x54321");

	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);

	printf("%ld: [%d] [%s]\n", (t = read_token(in_port))->position, t->type,
	       t->value);
	printf("%d at %ld: %s\n", lexer_errored, lexer_error_position,
	       lexer_error_message);

	printf("%ld: [%d] [%s]\n", (t = read_token(open_input_string("555")))->position, t->type,
	       t->value);
	printf("%d at %ld: %s\n", lexer_errored, lexer_error_position,
	       lexer_error_message);

	close_input_port(in_port);

	return 0;
}
