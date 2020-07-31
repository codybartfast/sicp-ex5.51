#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"

#define area "LEXER"
#define error_len 100

static enum token_type scan(struct in_port *);
static enum token_type number(struct in_port *);
static int add(int c);
bool peek_delimited(struct in_port *in);
void lexical_error(long position, char *msg);

static int buffidx, maxlen = (1 << 2) - 1;
static char *buff = NULL;
static struct token token;
static char error_msg[error_len];

struct token *read_token(struct in_port *in)
{
	if (buff == NULL) {
		buff = (char *)malloc((maxlen + 1) * sizeof(char));
		if (buff == NULL) {
			error(area, "No memory for buff");
			return NULL;
		}
	}
	buffidx = 0;
	token.value = buff;
	token.type = scan(in);
	*(buff + buffidx) = '\0';
	return &token;
}

enum token_type scan(struct in_port *in)
{
	int c;

	while (isspace(peek_char(in)))
		read_char(in);
	if ((c = peek_char(in)) == EOF)
		return TKN_EOF;
	token.position = in->read_count;
	if (isdigit(c))
		return number(in);
	sprintf(error_msg, "Unexpected start of datum: '%c' (%d)", c, c);
	lexical_error(in->read_count, error_msg);
	return EOF;
}

enum token_type number(struct in_port *in)
{
	while (isdigit(peek_char(in)))
		add(read_char(in));
	if (!peek_delimited(in)) {
		int c = peek_char(in);
		sprintf(error_msg, "Unexpect char in number: '%c' (%d)", c, c);
		lexical_error(in->read_count, error_msg);
		return EOF;
	}
	return TKN_NUMBER;
}

bool peek_delimited(struct in_port *in)
{
	int c = peek_char(in);

	if (isspace(c))
		return true;
	switch (c) {
	case '(':
	case ')':
	case '"':
	case ';':
	case EOF:
		return true;
	default:
		return false;
	}
}

int add(int c)
{
	if (buffidx >= maxlen) {
		error(area, "What! More than 3 chars?");
		exit(1);
	}
	return *(buff + buffidx++) = c;
}

void lexical_error(long position, char *msg)
{
	lexer_errored = true;
	lexer_error_position = position;
	lexer_error_message = msg;
	error(area, msg);
}
