#ifndef INPORT_H
#define INPORT_H
#include "sicpstd.h"

#include <stdio.h>

struct inport {
	int kind;
	char *name;
	FILE *file;
	char *text;
	char *next;
	int peeked;
	long read_count;

	int (*readc)(struct inport *);
	int (*peek)(struct inport *);
	int (*close)(struct inport *);
};

struct inport *openin_ptr(FILE *);
struct inport *openin_file(char *);
struct inport *openin_string(char *);

#endif
