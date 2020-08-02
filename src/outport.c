#include <stdio.h>
#include <stdlib.h>
#include "sserror.h"
#include "outport.h"

#define AREA "out_port"

enum { OUT_PORT_FILE_POINTER, OUT_PORT_FILE, OUT_PORT_STRING };

static struct out_port *new_out_port(void);

static void setfuncs(struct out_port *);

struct out_port *openout_ptr(FILE *file)
{
	struct out_port *op;
	if (file == NULL) {
		error(AREA, "open_output_file_pointer given a null file.");
		return NULL;
	}
	op = new_out_port();
	if (op == NULL)
		return NULL;
	op->kind = OUT_PORT_FILE_POINTER;
	op->name = NULL;
	op->file = file;
	op->sb = NULL;
	op->write_count = 0L;
	setfuncs(op);
	return op;
}

struct out_port *openout_file(char *name)
{
	FILE *file;
	struct out_port *op;

	if (name == NULL) {
		error(AREA, "open_output_file given a null name.");
		return NULL;
	}
	file = fopen(name, "w");
	if (file == NULL) {
		error(AREA, "failed to open file: '%s'", name);
		return NULL;
	}
	op = new_out_port();
	if (op == NULL)
		return NULL;

	op->kind = OUT_PORT_FILE;
	op->name = name;
	op->file = file;
	op->sb = NULL;
	op->write_count = 0L;
	setfuncs(op);
	return op;
}

struct out_port *openout_string()
{
	struct out_port *op;
	op = new_out_port();
	if (op == NULL)
		return NULL;
	op->kind = OUT_PORT_STRING;
	op->name = NULL;
	op->file = NULL;
	if ((op->sb = new_strbldr()) == NULL)
		return NULL;
	op->write_count = 0L;
	setfuncs(op);
	return op;
}

static int close_output_port(struct out_port *op)
{
	int rc = 0;
	if (op == NULL)
		return rc;
	// Close the FIlE only if it's one we openned
	if (op->file != NULL && op->kind == OUT_PORT_FILE)
		if ((rc = fclose(op->file)) == EOF)
			error(AREA, "error closing file: '%s'.", op->name);
	free(op);
	return rc;
}

static int op_writec(struct out_port *op, char c)
{
	if (op == NULL) {
		error(AREA, "writec received a null port.");
		return EOF;
	}
	switch (op->kind) {
	case OUT_PORT_FILE_POINTER:
	case OUT_PORT_FILE:
		op->write_count++;
		return putc(c, op->file);
	case OUT_PORT_STRING:
		op->write_count++;
		return op->sb->addc(op->sb, c);
	default:
		error(AREA, "BUG! More enums than cases.");
		return EOF;
	}
}

static int op_writes(struct out_port *op, char *s)
{
	int rc;
	for(; *s != '\0'; s++)
		rc = op_writec(op, *s);
	return rc == EOF ? EOF : 0;
}

static char *op_string(struct out_port *op)
{
	if (op == NULL)
		return NULL;
	if(op->kind != OUT_PORT_STRING){
		error(AREA, "Attempted to get string from non-string output");
		return NULL;
	}
	return op->sb->string(op->sb);
}

struct out_port *new_out_port(void)
{
	struct out_port *op =
		(struct out_port *)malloc(sizeof(struct out_port));
	if (op == NULL)
		error(AREA, "no memory for out_port struct.");
	return op;
}

static void setfuncs(struct out_port *op)
{
	op->writec = op_writec;
	op->writes = op_writes;
	op->string = op_string;
	op->close = close_output_port;
}
