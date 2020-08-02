#include <stdio.h>
#include <stdlib.h>
#include "sserror.h"
#include "inport.h"

#define NO_PEEK -2
#define AREA "in_port"

enum { IN_PORT_FILE_POINTER, IN_PORT_FILE, IN_PORT_STRING };

static struct in_port *new_in_port(void);

static void setfuncs(struct in_port *);

struct in_port *openin_ptr(FILE *file)
{
	struct in_port *ip;
	if (file == NULL) {
		error(AREA, "open_input_file_pointer given a null file.");
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
	setfuncs(ip);
	return ip;
}

struct in_port *openin_file(char *name)
{
	FILE *file;
	struct in_port *ip;

	if (name == NULL) {
		error(AREA, "open_input_file given a null name.");
		return NULL;
	}
	file = fopen(name, "r");
	if (file == NULL) {
		error(AREA, "failed to open file: '%s'", name);
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
	setfuncs(ip);
	return ip;
}

struct in_port *openin_string(char *text)
{
	struct in_port *ip;
	if (text == NULL) {
		error(AREA, "open_input_string given a null string.");
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
	setfuncs(ip);
	return ip;
}

static int close_input_port(struct in_port *ip)
{
	int rc = 0;
	if (ip == NULL)
		return rc;
	// Close the FIlE only if it's one we openned
	if (ip->file != NULL && ip->kind == IN_PORT_FILE)
		if ((rc = fclose(ip->file)) == EOF)
			error(AREA, "error closing file: '%s'.", ip->name);
	free(ip);
	return rc;
}

static int direct_read(struct in_port *ip)
{
	if (ip == NULL) {
		error(AREA, "read_char received a null port.");
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
		error(AREA, "BUG! More enums than cases.");
		return EOF;
	}
}

static int read_char(struct in_port *ip)
{
	if (ip->peeked == NO_PEEK)
		return direct_read(ip);
	else {
		int c = ip->peeked;
		ip->peeked = NO_PEEK;
		return c;
	}
}

static int peek_char(struct in_port *ip)
{
	if (ip->peeked == NO_PEEK)
		ip->peeked = direct_read(ip);
	return ip->peeked;
}

struct in_port *new_in_port(void)
{
	struct in_port *ip = (struct in_port *)malloc(sizeof(struct in_port));
	if (ip == NULL)
		error(AREA, "no memory for in_port struct.");
	return ip;
}

static void setfuncs(struct in_port *ip){
	ip->readc = read_char;
	ip->peek = peek_char;
	ip->close = close_input_port;
}
