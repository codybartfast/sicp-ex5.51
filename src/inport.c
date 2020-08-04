#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "inport.h"
#include "windows.h"

#define NO_PEEK -2
#define AREA "in_port"

enum inkind { INPORT_FILE_POINTER = 1, INPORT_FILE, INPORT_STRING };

static struct inport *new_inport(void);

struct inport *openin_ptr(FILE *file)
{
	struct inport *ip;
	if (file == NULL) {
		error(AREA, "open_input_file_pointer given a null file.");
		return NULL;
	}
	ip = new_inport();
	if (ip == NULL)
		return NULL;
	ip->kind = INPORT_FILE_POINTER;
	ip->file = file;
	return ip;
}

struct inport *openin_file(char *name)
{
	FILE *file;
	struct inport *in;

	if (name == NULL) {
		error(AREA, "open_input_file given a null name.");
		return NULL;
	}
	if (fopen_s(&file, name, "r")) {
		error(AREA, "failed to open file: '%s'", name);
		return NULL;
	}
	in = new_inport();
	if (in == NULL)
		return NULL;

	in->kind = INPORT_FILE;
	in->name = name;
	in->file = file;
	return in;
}

struct inport *openin_string(char *text)
{
	struct inport *in;
	if (text == NULL) {
		error(AREA, "open_input_string given a null string.");
		return NULL;
	}
	in = new_inport();
	if (in == NULL)
		return NULL;
	in->kind = INPORT_STRING;
	return in;
}

static int close_inport(struct inport *in)
{
	int rc = 0;
	if (in == NULL)
		return rc;
	if (in->file != NULL && in->kind == INPORT_FILE)
		if ((rc = fclose(in->file)) == EOF)
			error(AREA, "error closing file: '%s'.", in->name);
	free(in);
	return rc;
}

static int direct_read(struct inport *in)
{
	if (in == NULL) {
		error(AREA, "read_char received a null port.");
		return EOF;
	}
	switch (in->kind) {
	case INPORT_FILE_POINTER:
	case INPORT_FILE:
		in->read_count++;
		return getc(in->file);
	case INPORT_STRING:
		in->read_count++;
		return (*in->next == '\0') ? EOF : *(in->next++);
	default:
		error(AREA, "BUG! More enums than cases.");
		return EOF;
	}
}

static int read_char(struct inport *in)
{
	if (in->peeked == NO_PEEK)
		return direct_read(in);
	else {
		int c = in->peeked;
		in->peeked = NO_PEEK;
		return c;
	}
}

static int peek_char(struct inport *in)
{
	if (in->peeked == NO_PEEK)
		in->peeked = direct_read(in);
	return in->peeked;
}

struct inport *new_inport(void)
{
	struct inport *in = (struct inport *)malloc(sizeof(struct inport));
	if (in == NULL) {
		error(AREA, "no memory for in_port struct.");
		return NULL;
	}
	*in = (struct inport){ 0 };
	in->peeked = NO_PEEK;

	in->readc = read_char;
	in->peek = peek_char;
	in->close = close_inport;
	return in;
}
