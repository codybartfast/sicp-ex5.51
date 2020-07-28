#include <stdio.h>
#include "source.h"

void parse(source *);

int main(void)
{
	source *source;

	printf("\nreading source from NAMED file:\n    ");
	source = sopen_name("file.txt");
	parse(source);
	sclose(source);

	printf("\nreading source from a STRING:\n    ");
	source = sopen_string("(i am a string)");
	parse(source);
	sclose(source);

	printf("\n\nreading source from FILE pointer:\n    ");
	source = sopen_file(stdin);
	parse(source);
	sclose(source);
	printf("\n");
	
	return 0;
}

void parse(source *s)
{
	int c;
	while ((c = sgetc(s)) != EOF)
		putchar(c);
}
