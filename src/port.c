#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "port.h"

#define NO_PEEK -2
#define area "PORT"

static struct in_port *new_in_port(void);
static int read(struct in_port *);

struct in_port *open_input_file_pointer(FILE *file)
{
	struct in_port *ip;
	if (file == NULL) {
		error(area, "open_input_file_pointer given a null file.");
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
	ip->peeked = NO_PEEK;
	ip->read_count = 0L;
	return ip;
}

struct in_port *open_input_file(char *name)
{
	FILE *file;
	struct in_port *ip;

	if (name == NULL) {
		error(area, "open_input_file given a null name.");
		return NULL;
	}
	file = fopen(name, "r");
	if (file == NULL) {
		error(area, "failed to open file: '%s'", name);
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
	ip->peeked = NO_PEEK;
	ip->read_count = 0L;
	return ip;
}

struct in_port *open_input_string(char *text)
{
	struct in_port *ip;
	if (text == NULL) {
		error(area, "open_input_string given a null string.");
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
	ip->peeked = NO_PEEK;
	ip->read_count = 0L;
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
			error(area, "error closing file: '%s'.", ip->name);
	free(ip);
	return rc;
}

int read(struct in_port *ip)
{
	if (ip == NULL) {
		error(area, "read_char received a null port.");
		return EOF;
	}
	switch (ip->kind) {
	case IN_PORT_FILE_POINTER:
	case IN_PORT_FILE:
		ip->read_count++;
		return getc(ip->file);
	case IN_PORT_STRING:
		ip->read_count++;
		return (*ip->next == '\0') ? EOF : *(ip->next++);
	default:
		error(area, "BUG! More enums than cases.");
		return EOF;
	}
}

int read_char(struct in_port *ip)
{
	if (ip->peeked == NO_PEEK)
		return read(ip);
	else {
		int c = ip->peeked;
		ip->peeked = NO_PEEK;
		return c;
	}
}

int peek_char(struct in_port *ip)
{
	if (ip->peeked == NO_PEEK)
		ip->peeked = read(ip);
	return ip->peeked;
}

struct in_port *new_in_port(void)
{
	struct in_port *ip = (struct in_port *)malloc(sizeof(struct in_port));
	if (ip == NULL)
		error(area, "no memory for source struct.");
	return ip;
}
