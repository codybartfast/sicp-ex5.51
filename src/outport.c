#include "outport.h"

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define AREA "OUTPORT"

enum { OUTKIND_PTR = 1, OUTKIND_FILE, OUTKIND_STRING };

static struct outport *new_outport(void);

struct outport *openout_ptr(FILE *file)
{
	struct outport *port;
	if (file == NULL) {
		eprintf(AREA, "openout_ptr given a null file pointer.");
		return NULL;
	}
	if ((port = new_outport()) == NULL)
		return NULL;
	port->kind = OUTKIND_PTR;
	port->file = file;
	return port;
}

struct outport *openout_file(char *name)
{
	FILE *file;
	struct outport *port;

	if (name == NULL) {
		eprintf(AREA, "openout_file given a null filename.");
		return NULL;
	}
	if ((file = fopen(name, "w")) == NULL) {
		eprintf(AREA, "failed to open file: '%s'", name);
		return NULL;
	}
	if ((port = new_outport()) == NULL)
		return NULL;
	port->kind = OUTKIND_FILE;
	port->name = name;
	port->file = file;
	return port;
}

struct outport *openout_string(void)
{
	struct outport *port;
	if ((port = new_outport()) == NULL)
		return NULL;
	port->kind = OUTKIND_STRING;
	if ((port->sb = new_strbldr()) == NULL)
		return NULL;
	return port;
}

static int close(struct outport *port)
{
	int rc = 0;
	if (port == NULL)
		return rc;
	if (port->file != NULL && port->kind == OUTKIND_FILE)
		if ((rc = fclose(port->file)) == EOF)
			eprintf(AREA, "error closing file: '%s'.", port->name);
	if (port != NULL)
		port->sb->free(&port->sb);
	free(port);
	return rc;
}

static int writec(struct outport *port, char c)
{
	if (port == NULL) {
		eprintf(AREA, "writec received a null outport.");
		return EOF;
	}
	switch (port->kind) {
	case OUTKIND_PTR:
	case OUTKIND_FILE:
		port->write_count++;
		return putc(c, port->file);
	case OUTKIND_STRING:
		port->write_count++;
		return port->sb->addc(port->sb, c);
	default:
		eprintf(AREA, "BUG! No case for outkind: %d", port->kind);
		return EOF;
	}
}

static int writes(struct outport *out, const char *str)
{
	int rc = 0;
	for (; *str != '\0' && rc >= 0; str++)
		rc = writec(out, *str);
	return rc < 0 ? EOF : 0;
}

static char *tostring(struct outport *port)
{
	if (port == NULL) {
		eprintf(AREA, "tostring received a null outport.");
		return NULL;
	}
	if (port->kind != OUTKIND_STRING) {
		eprintf(AREA, "cannot get string from a non-string outport");
		return NULL;
	}
	return port->sb->string(port->sb);
}

struct outport *new_outport(void)
{
	struct outport *port = (struct outport *)malloc(sizeof(struct outport));
	if (port == NULL) {
		eprintf(AREA, "no memory for out_port struct");
		return NULL;
	}
	*port = (struct outport){ 0 };

	port->writec = writec;
	port->writes = writes;
	port->tostring = tostring;
	port->close = close;

	return port;
}
