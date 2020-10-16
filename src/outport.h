#ifndef OUTPORT_H
#define OUTPORT_H
#include "sicpstd.h"

#include <stdio.h>
#include "strbldr.h"

struct outport {
	int kind;
	char *name;
	FILE *file;
	struct strbldr *sb;
	long write_count;
};

struct outport *openout_ptr(FILE *);
struct outport *openout_file(char *);
struct outport *openout_string(void);

int out_writec(struct outport *, char c);
int out_writes(struct outport *, const char *s);
char *out_copystring(struct outport *);
int out_close(struct outport *);

#endif
