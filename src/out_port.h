#ifndef OUT_PORT_H
#define OUT_PORT_H 1

#include <stdio.h>
#include "strbuild.h"

struct out_port {
	int kind;
	char *name;
	FILE *file;
	struct strbuild *sb;
	long write_count;

	int (*writec)(struct out_port *, char c);
	int (*writes)(struct out_port *, char *s);
	char *(*string)(struct out_port *);
	int (*close)(struct out_port *);
};

struct out_port *open_output_file_pointer(FILE *);
struct out_port *open_output_file(char *);
struct out_port *open_output_string(void);

#endif
