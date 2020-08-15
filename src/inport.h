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
};

struct inport *openin_ptr(FILE *);
struct inport *openin_file(char *);
struct inport *openin_string(char *);

int in_readc(struct inport *);
int in_peek(struct inport *);
int in_close(struct inport *);

#endif
