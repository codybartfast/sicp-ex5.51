#include <stdio.h>
#include <stdlib.h>
#include "source.h"

static source *new_source(void);

source *sopen_file(FILE *file)
{
	source *s;
	if (file == NULL) {
		fprintf(stderr, "SOURCE: sopen_file given a null file.\n");
		return NULL;
	}
	s = new_source();
	if (s == NULL)
		return NULL;
	s->kind = SOURCE_FILE_POINTER;
	s->name = NULL;
	s->file = file;
	s->text = NULL;
	s->next = NULL;
	return s;
}

source *sopen_name(char *name)
{
	FILE *file;
	source *s;

	if (name == NULL) {
		fprintf(stderr, "SOURCE: sopen_name given a null name.\n");
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

	s->kind = SOURCE_FILE_NAME;
	s->name = name;
	s->file = file;
	s->text = NULL;
	s->next = NULL;
	return s;
}

source *sopen_string(char *text)
{
	source *s;
	if (text == NULL) {
		fprintf(stderr,
			"SOURCE: sopen_string given a null string.\n");
		return NULL;
	}
	s = new_source();
	if (s == NULL)
		return NULL;
	s->kind = SOURCE_STRING;
	s->name = NULL;
	s->file = NULL;
	s->text = text;
	s->next = text;
	return s;
}

int sclose(source *s)
{
	int rc = 0;
	if (s == NULL)
		return rc;
	// Close the FIlE if it's one we openned
	if (s->file != NULL && s->kind == SOURCE_FILE_NAME)
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
	case SOURCE_FILE_POINTER:
	case SOURCE_FILE_NAME:
		return getc(s->file);
	case SOURCE_STRING:
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
