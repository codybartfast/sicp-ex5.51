/*
 * Why?  Expect to be reading source from a file, but for other use case, e.g.,
 * testing and dev, it will be useful to parse a string directly.  sgetc(source)
 * is intended to be used in the same way as getc(file).
 *
 * Could use fmemopen, but it's not portable.
 *
 * The block struct could use a union, but I'm not sure that gives anything
 * other than saving a few bytes.
 */

#include <stdio.h>

enum src_kind { SRC_FILE_POINTER, SRC_FILE_NAME, SRC_STRING };

typedef struct source {
	enum src_kind kind;
	char *name;
	FILE *file;
	char *text;
	char *next;
} source;

source *src_file(FILE *);
source *src_name(char *);
source *src_string(char *);

int src_close(source *);

int sgetc(source *);
