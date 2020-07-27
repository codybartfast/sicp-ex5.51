#include <stdio.h>
#include "source.h"

void parse(source *);

int main(void)
{
	source *source;

	printf("\nreading source from NAMED file:\n    ");
	source = source_name("file.txt");
	parse(source);
	source_close(source);

	printf("\nreading source from a STRING:\n    ");
	source = source_string("I am a string");
	parse(source);
	source_close(source);

	printf("\n\nreading source from FILE pointer:\n    ");
	source = source_file(stdin);
	parse(source);
	source_close(source);
	printf("\n");
	
	return 0;
}

void parse(source *s)
{
	int c;
	while ((c = sgetc(s)) != EOF)
		putchar(c);
}
