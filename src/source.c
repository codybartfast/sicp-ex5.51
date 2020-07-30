#include <stdio.h>
#include <stdlib.h>
#include "source.h"

static source *new_source(void);

source *src_file(FILE *file)
{
	source *s;
	if (file == NULL) {
		fprintf(stderr, "SOURCE: src_file given a null file.\n");
		return NULL;
	}
	s = new_source();
	if (s == NULL)
		return NULL;
	s->kind = SRC_FILE_POINTER;
	s->name = NULL;
	s->file = file;
	s->text = NULL;
	s->next = NULL;
	return s;
}

source *src_name(char *name)
{
	FILE *file;
	source *s;

	if (name == NULL) {
		fprintf(stderr, "SOURCE: src_name given a null name.\n");
		return NULL;
	}
	file = fopen(name, "r");
	if (file == NULL) {
		fprintf(stderr, "SOURCE: failed to open file: '%s'\n", name);
		return NULL;
	}
	s = new_source();
	if (s == NULL)
		return NULL;

	s->kind = SRC_FILE_NAME;
	s->name = name;
	s->file = file;
	s->text = NULL;
	s->next = NULL;
	return s;
}

source *src_string(char *text)
{
	source *s;
	if (text == NULL) {
		fprintf(stderr, "SOURCE: src_string given a null string.\n");
		return NULL;
	}
	s = new_source();
	if (s == NULL)
		return NULL;
	s->kind = SRC_STRING;
	s->name = NULL;
	s->file = NULL;
	s->text = text;
	s->next = text;
	return s;
}

int src_close(source *s)
{
	int rc = 0;
	if (s == NULL)
		return rc;
	// Close the FIlE only if it's one we openned
	if (s->file != NULL && s->kind == SRC_FILE_NAME)
		if ((rc = fclose(s->file)) == EOF)
			fprintf(stderr, "SOURCE: error closing file: '%s'.\n",
				s->name);
	free(s);
	return rc;
}

int sgetc(source *s)
{
	if (s == NULL) {
		fprintf(stderr, "SOURCE: sgetc received a null source.\n");
		return EOF;
	}
	switch (s->kind) {
	case SRC_FILE_POINTER:
	case SRC_FILE_NAME:
		return getc(s->file);
	case SRC_STRING:
		return (*s->next == '\0') ? EOF : *(s->next++);
	default:
		fprintf(stderr, "SOURCE: BUG! More enums than cases.\n");
		return EOF;
	}
}

source *new_source(void)
{
	source *s = (source *)malloc(sizeof(source));
	if (s == NULL)
		fprintf(stdin, "SOURCE: no memory for source struct.\n");
	return s;
}
