#ifndef OUTPORT_H
#define OUTPORT_H 1

#include <stdio.h>
#include "strbldr.h"

struct outport {
	int kind;
	char *name;
	FILE *file;
	struct strbldr *sb;
	long write_count;

	int (*writec)(struct outport *, char c);
	int (*writes)(struct outport *, char *s);
	char *(*tostring)(struct outport *);
	int (*close)(struct outport *);
};

struct outport *openout_ptr(FILE *);
struct outport *openout_file(char *);
struct outport *openout_string(void);

#endif
