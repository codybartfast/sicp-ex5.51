#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strbuild.h"
#include "sserror.h"
#include "lexer.h"

#define AREA "LEXER"
#define error_len 100

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct in_port *);
static enum token_type number(struct in_port *);
static bool peek_delimited(struct in_port *in);
static void lexical_error(long position, char *msg);

static struct strbuild *sb;
static struct token token;
static char error_msg[error_len];

struct token *read_token(struct in_port *in)
{
	if (lexer_errored != false) {
		lexer_errored = false;
		lexer_error_position = -1;
		lexer_error_message = NULL;
	}
	if (sb == NULL) {
		sb = new_strbuild();
		if (sb == NULL) {
			error(AREA, "No memory for strbuild");
			return NULL;
		}
	}
	token.type = scan(in);
	token.value = sb->string(sb);

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
	while (isdigit(peek_char(in))) {
		if (sb->addc(sb, read_char(in)) == TKN_EOF)
			return TKN_EOF;
	}
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

void lexical_error(long position, char *msg)
{
	lexer_errored = true;
	lexer_error_position = position;
	lexer_error_message = msg;
	error(AREA, msg);
}
