#include <stdio.h>
#include "port.h"

void parse(struct in_port *);

int main(void)
{
	struct in_port *in_port;

	printf("\nreading source from NAMED file:\n    ");
	in_port = open_input_file("../test/file.txt");
	parse(in_port);
	close_input_port(in_port);

	printf("\nreading source from a STRING:\n    ");
	in_port = open_input_string("(i am a string)");
	parse(in_port);
	close_input_port(in_port);

	printf("\n\nreading source from FILE pointer:\n    ");
	in_port = open_input_file_pointer(stdin);
	parse(in_port);
	close_input_port(in_port);
	printf("\n");
	
	return 0;
}

void parse(struct in_port *ip)
{
	int c;
	while ((c = read_char(ip)) != EOF)
		putchar(c);
}

/* output

sicp-scheme/ [dev ≡ +1 ~0 -0]> echo "(comming down the pipe)" | a.out

reading source from NAMED file:
    (stuff in a file)

reading source from a STRING:
    (i am a string)

reading source from FILE pointer:
    (comming down the pipe)

sicp-scheme/ [dev ≡ +1 ~0 -0]> 

*/
