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

enum source_kind { SOURCE_FILE_POINTER, SOURCE_FILE_NAME, SOURCE_STRING };

typedef struct {
	enum source_kind kind;
	char *name;
	FILE *file;
	char *text;
	char *next;
} source;

source *sopen_file(FILE *);
source *sopen_name(char *);
source *sopen_string(char *);

int sclose(source *);

int sgetc(source *);
