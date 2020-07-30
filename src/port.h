/*
 * Could use fmemopen, but it's not portable.
 */

#include <stdio.h>

enum in_port_kind { IN_PORT_FILE_POINTER, IN_PORT_FILE, IN_PORT_STRING };

struct in_port{
	enum in_port_kind kind;
	char *name;
	FILE *file;
	char *text;
	char *next;
	int peeked;
	long read_count;
};

struct in_port *open_input_file_pointer(FILE *);
struct in_port *open_input_file(char *);
struct in_port *open_input_string(char *);

int close_input_port(struct in_port *);

int read_char(struct in_port *);
int peek_char(struct in_port *);
