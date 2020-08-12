#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strbldr.h"
#include "error.h"
#include "windows.h"

#define AREA "LEXER"
#define MAXERROR 100

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct inport *);
enum token_type peculiar(char c, struct inport *in);
static enum token_type number(char c, struct inport *);
static bool peek_delimited(struct inport *in);
static void lexer_error(long position, char *msg);

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
	if (sb->errored) {
		token.type = TKN_EOF;
		lexer_error(in->read_count, "strbldr (memory) error");
	}
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
	if (c == '+' || c == '-')
		return peculiar(c, in);
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
	lexer_error(in->read_count, error_msg);
	return EOF;
}

enum token_type peculiar(char c, struct inport *in)
{
	if (peek_delimited(in)) {
		sb->addc(sb, c);
		return TKN_IDENTIFIER;
	}
	if (isdigit(in->peek(in))) {
		return number(c, in);
	}
	char p = in->peek(in);
	sprintf_s(error_msg, MAXERROR,
		  "Unexpected char following initial '%c': '%c' (%d)", c, p, p);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

enum token_type number(char c, struct inport *in)
{
	sb->addc(sb, c);
	while (isdigit(in->peek(in))) {
		if (sb->addc(sb, in->readc(in)) == EOF)
			return TKN_EOF;
	}
	if (!peek_delimited(in)) {
		int c = in->peek(in);
		sprintf_s(error_msg, MAXERROR,
			  "Unexpect char in number: '%c' (%d)", c, c);
		lexer_error(in->read_count, error_msg);
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

void lexer_freetemp(void)
{
	sb->free(&sb);
}

void lexer_error(long position, char *msg)
{
	lexer_errored = true;
	lexer_error_position = position;
	lexer_error_message = msg;
	eprintf(AREA, msg);
}
