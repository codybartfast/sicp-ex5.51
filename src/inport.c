#include "inport.h"

#include <stdlib.h>
#include "error.h"

#define NO_PEEK -2
#define AREA "INPORT"

enum inkind { INKIND_PTR = 1, INKIND_FILE, INKIND_STRING };

static struct inport *new_inport(void);

struct inport *openin_ptr(FILE *file)
{
	struct inport *port;
	if (file == NULL) {
		eprintf(AREA, "openin_ptr given a null file pointer.");
		return NULL;
	}
	if ((port = new_inport()) == NULL)
		return NULL;
	port->kind = INKIND_PTR;
	port->file = file;
	return port;
}

struct inport *openin_file(char *name)
{
	FILE *file;
	struct inport *port;

	if (name == NULL) {
		eprintf(AREA, "openin_file given a null filename.");
		return NULL;
	}
	if ((file = fopen(name, "r")) == NULL) {
		eprintf(AREA, "failed to open file: \"%s\"", name);
		return NULL;
	}
	if ((port = new_inport()) == NULL)
		return NULL;
	port->kind = INKIND_FILE;
	port->name = name;
	port->file = file;
	return port;
}

struct inport *openin_string(char *text)
{
	struct inport *port;
	if (text == NULL) {
		eprintf(AREA, "openin_string given a null string.");
		return NULL;
	}
	if ((port = new_inport()) == NULL)
		return NULL;
	port->kind = INKIND_STRING;
	port->next = port->text = text;
	return port;
}

int in_close(struct inport *port)
{
	int rc = 0;
	if (port == NULL)
		return rc;
	if (port->file != NULL && port->kind == INKIND_FILE)
		if ((rc = fclose(port->file)) == EOF)
			eprintf(AREA, "error closing file: '%s'.", port->name);
	free(port);
	return rc;
}

static int direct_read(struct inport *port)
{
	switch (port->kind) {
	case INKIND_PTR:
	case INKIND_FILE:
		port->read_count++;
		return getc(port->file);
	case INKIND_STRING:
		port->read_count++;
		return (*port->next == '\0') ? EOF : *(port->next++);
	default:
		eprintf(AREA, "BUG! No case for inport kind: %d", port->kind);
		return EOF;
	}
}

int in_readc(struct inport *port)
{
	char c;
	if (port == NULL) {
		eprintf(AREA, "read_char received a null port.");
		return EOF;
	}
	if (port->peeked == NO_PEEK)
		c = direct_read(port);
	else {
		c = port->peeked;
		port->peeked = NO_PEEK;
	}
	port->offset++;
	if (c == '\n') {
		port->line++;
		port->column = 0;
	} else {
		port->column++;
	}
	return c;
}

int in_peek(struct inport *port)
{
	if (port == NULL) {
		eprintf(AREA, "peek_char received a null port.");
		return EOF;
	}

	if (port->peeked == NO_PEEK)
		port->peeked = direct_read(port);
	return port->peeked;
}

struct inport *new_inport(void)
{
	struct inport *port = (struct inport *)calloc(sizeof(struct inport), 1);
	if (port == NULL) {
		eprintf(AREA, "no memory for inport struct");
		return NULL;
	}
	port->peeked = NO_PEEK;
	return port;
}
