#include <stdio.h>
#include <stdlib.h>
#include "port.h"

static struct in_port *new_in_port(void);

struct in_port *open_input_file_pointer(FILE *file)
{
	struct in_port *ip;
	if (file == NULL) {
		fprintf(stderr,
			"PORT: open_input_file_pointer given a null file.\n");
		return NULL;
	}
	ip = new_in_port();
	if (ip == NULL)
		return NULL;
	ip->kind = IN_PORT_FILE_POINTER;
	ip->name = NULL;
	ip->file = file;
	ip->text = NULL;
	ip->next = NULL;
	return ip;
}

struct in_port *open_input_file(char *name)
{
	FILE *file;
	struct in_port *ip;

	if (name == NULL) {
		fprintf(stderr, "PORT: open_input_file given a null name.\n");
		return NULL;
	}
	file = fopen(name, "r");
	if (file == NULL) {
		fprintf(stderr, "PORT: failed to open file: '%s'\n", name);
		return NULL;
	}
	ip = new_in_port();
	if (ip == NULL)
		return NULL;

	ip->kind = IN_PORT_FILE;
	ip->name = name;
	ip->file = file;
	ip->text = NULL;
	ip->next = NULL;
	return ip;
}

struct in_port *open_input_string(char *text)
{
	struct in_port *ip;
	if (text == NULL) {
		fprintf(stderr,
			"PORT: open_input_string given a null string.\n");
		return NULL;
	}
	ip = new_in_port();
	if (ip == NULL)
		return NULL;
	ip->kind = IN_PORT_STRING;
	ip->name = NULL;
	ip->file = NULL;
	ip->text = text;
	ip->next = text;
	return ip;
}

int close_input_port(struct in_port *ip)
{
	int rc = 0;
	if (ip == NULL)
		return rc;
	// Close the FIlE only if it's one we openned
	if (ip->file != NULL && ip->kind == IN_PORT_FILE)
		if ((rc = fclose(ip->file)) == EOF)
			fprintf(stderr, "PORT: error closing file: '%s'.\n",
				ip->name);
	free(ip);
	return rc;
}

int read_char(struct in_port *ip)
{
	if (ip == NULL) {
		fprintf(stderr, "PORT: read_char received a null port.\n");
		return EOF;
	}
	switch (ip->kind) {
	case IN_PORT_FILE_POINTER:
	case IN_PORT_FILE:
		return getc(ip->file);
	case IN_PORT_STRING:
		return (*ip->next == '\0') ? EOF : *(ip->next++);
	default:
		fprintf(stderr, "PORT: BUG! More enums than cases.\n");
		return EOF;
	}
}

struct in_port *new_in_port(void)
{
	struct in_port *ip = (struct in_port *)malloc(sizeof(struct in_port));
	if (ip == NULL)
		fprintf(stdin, "PORT: no memory for source struct.\n");
	return ip;
}
