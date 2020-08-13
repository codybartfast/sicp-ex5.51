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

static int close(struct inport *port)
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

static int readc(struct inport *port)
{
	if (port == NULL) {
		eprintf(AREA, "read_char received a null port.");
		return EOF;
	}
	if (port->peeked == NO_PEEK)
		return direct_read(port);
	else {
		int c = port->peeked;
		port->peeked = NO_PEEK;
		return c;
	}
}

static int peek(struct inport *port)
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
	struct inport *port = (struct inport *)malloc(sizeof(struct inport));
	if (port == NULL) {
		eprintf(AREA, "no memory for inport struct");
		return NULL;
	}
	*port = (struct inport){ 0 };
	port->peeked = NO_PEEK;
	port->readc = readc;
	port->peek = peek;
	port->close = close;
	return port;
}
