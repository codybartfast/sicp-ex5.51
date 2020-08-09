#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strbldr.h"
#include "error.h"
#include "lexer.h"
#include "windows.h"

#define AREA "LEXER"
#define MAXERROR 100

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct inport *);
static enum token_type number(char c, struct inport *);
static bool peek_delimited(struct inport *in);
static void lexical_error(long position, char *msg);

static struct strbldr *sb = NULL;
static struct token token;
static char error_msg[MAXERROR];

struct token *read_token(struct inport *in)
{
	if (lexer_errored != false) {
		lexer_errored = false;
		lexer_error_position = -1;
		lexer_error_message = NULL;
	}
	if (sb == NULL) {
		sb = new_strbldr();
		if (sb == NULL) {
			eprintf(AREA, "No memory for strbuild");
			return NULL;
		}
	} else {
		sb->clear(sb);
	}
	token.type = scan(in);
	token.value = sb->string(sb);
	return &token;
}

enum token_type scan(struct inport *in)
{
	int c;
	while (isspace(in->peek(in)))
		in->readc(in);
	if ((c = in->readc(in)) == EOF) {
		return TKN_EOF;
	}
	token.position = in->read_count;
	if (isdigit(c))
		return number(c, in);
	if (c == '(') {
		sb->addc(sb, '(');
		return TKN_LIST_OPEN;
	}
	if (c == ')') {
		sb->addc(sb, ')');
		return TKN_LIST_CLOSE;
	}
	sprintf_s(error_msg, MAXERROR, "Unexpected start of datum: '%c' (%d)",
		  c, c);
	lexical_error(in->read_count, error_msg);
	return EOF;
}

enum token_type number(char c, struct inport *in)
{
	sb->addc(sb, c);
	while (isdigit(in->peek(in))) {
		if (sb->addc(sb, in->readc(in)) == TKN_EOF)
			return TKN_EOF;
	}
	if (!peek_delimited(in)) {
		int c = in->peek(in);
		sprintf_s(error_msg, MAXERROR,
			  "Unexpect char in number: '%c' (%d)", c, c);
		lexical_error(in->read_count, error_msg);
		return EOF;
	}
	return TKN_NUMBER;
}

bool peek_delimited(struct inport *in)
{
	int c = in->peek(in);

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
	eprintf(AREA, msg);
}
