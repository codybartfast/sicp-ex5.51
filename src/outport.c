#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "outport.h"
#include "windows.h"

#define AREA "OUTPORT"

enum { OUTPORT_FILE_POINTER = 1, OUTPORT_FILE, OUTPORT_STRING };

static struct outport *new_outport(void);

struct outport *openout_ptr(FILE *file)
{
	struct outport *out;
	if (file == NULL) {
		error(AREA, "open_output_file_pointer given a null file.");
		return NULL;
	}
	out = new_outport();
	if (out == NULL)
		return NULL;

	out->kind = OUTPORT_FILE_POINTER;
	out->file = file;
	return out;
}

struct outport *openout_file(char *name)
{
	FILE *file;
	struct outport *out;

	if (name == NULL) {
		error(AREA, "open_output_file given a null name.");
		return NULL;
	}

	if (fopen_s(&file, name, "w")) {
		error(AREA, "failed to open file: '%s'", name);
		return NULL;
	}
	out = new_outport();
	if (out == NULL)
		return NULL;

	out->kind = OUTPORT_FILE;
	out->name = name;
	out->file = file;
	return out;
}

struct outport *openout_string()
{
	struct outport *out;
	out = new_outport();
	if (out == NULL)
		return NULL;
	out->kind = OUTPORT_STRING;
	if ((out->sb = new_strbldr()) == NULL)
		return NULL;
	return out;
}

static int close_outport(struct outport *out)
{
	int rc = 0;
	if (out == NULL)
		return rc;
	if (out->file != NULL && out->kind == OUTPORT_FILE)
		if ((rc = fclose(out->file)) == EOF)
			error(AREA, "error closing file: '%s'.", out->name);
	free(out);
	return rc;
}

static int op_writec(struct outport *out, char c)
{
	if (out == NULL) {
		error(AREA, "writec received a null port.");
		return EOF;
	}
	switch (out->kind) {
	case OUTPORT_FILE_POINTER:
	case OUTPORT_FILE:
		out->write_count++;
		return putc(c, out->file);
	case OUTPORT_STRING:
		out->write_count++;
		return out->sb->addc(out->sb, c);
	default:
		error(AREA, "BUG! More enums than cases.");
		return EOF;
	}
}

static int op_writes(struct outport *out, char *str)
{
	int rc = 0;
	for (; *str != '\0'; str++)
		rc = op_writec(out, *str);
	return rc == EOF ? EOF : 0;
}

static char *op_string(struct outport *out)
{
	if (out == NULL)
		return NULL;
	if (out->kind != OUTPORT_STRING) {
		error(AREA, "Attempted to get string from non-string output");
		return NULL;
	}
	return out->sb->string(out->sb);
}

struct outport *new_outport(void)
{
	struct outport *out = (struct outport *)malloc(sizeof(struct outport));
	if (out == NULL) {
		error(AREA, "no memory for out_port struct.");
		return NULL;
	}
	*out = (struct outport){ 0 };

	out->writec = op_writec;
	out->writes = op_writes;
	out->string = op_string;
	out->close = close_outport;

	return out;
}
