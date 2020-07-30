/*
 * Why?  Expect to be reading source from a file, but for other use case, e.g.,
 * testing and dev, it will be useful to parse a string directly.  sgetc(source)
 * is intended to be used in the same way as getc(file).
 *
 * Could use fmemopen, but it's not portable.
 *
 * The block struct could use a union, but I'm not sure that gives anything
 * other than saving a few bytes.
 */

#include <stdio.h>

enum in_port_kind { IN_PORT_FILE_POINTER, IN_PORT_FILE, IN_PORT_STRING };

struct in_port{
	enum in_port_kind kind;
	char *name;
	FILE *file;
	char *text;
	char *next;
};

struct in_port *open_input_file_pointer(FILE *);
struct in_port *open_input_file(char *);
struct in_port *open_input_string(char *);

int close_input_port(struct in_port *);

int read_char(struct in_port *);
