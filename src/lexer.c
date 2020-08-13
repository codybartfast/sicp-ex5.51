#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "character.h"
#include "strbldr.h"
#include "error.h"
#include "windows.h"

#define AREA "LEXER"
#define MAXERROR 100

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct inport *);
static enum token_type identifier(char c, struct inport *);
static enum token_type number(char c, struct inport *);
enum token_type peculiar(char c, struct inport *in);
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

	while (is_whitespace(in->peek(in)))
		in->readc(in);
	if ((c = in->readc(in)) == EOF) {
		return TKN_EOF;
	}
	token.position = in->read_count;
	if (is_initial(c))
		return identifier(c, in);
	if (c == '(') {
		sb->addc(sb, '(');
		return TKN_LIST_OPEN;
	}
	if (c == ')') {
		sb->addc(sb, ')');
		return TKN_LIST_CLOSE;
	}
	if (is_peculiar_identifier(c))
		return peculiar(c, in);
	if (is_digit(c))
		return number(c, in);
	sprintf_s(error_msg, MAXERROR, "Unexpected start of datum: '%c' (%d)",
		  c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

enum token_type identifier(char c, struct inport *in)
{
	sb->addc(sb, c);
	while (is_subsequent(in->peek(in))) {
		sb->addc(sb, in->readc(in));
	}
	c = in->peek(in);
	if (c == EOF || is_delimiter(c)) {
		return TKN_IDENTIFIER;
	}
	sprintf_s(error_msg, MAXERROR,
		  "Unexpected char in identifier (%s...): %c' (%d)",
		  sb->string(sb), c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

enum token_type peculiar(char c, struct inport *in)
{
	if (is_delimiter(in->peek(in))) {
		sb->addc(sb, c);
		return TKN_IDENTIFIER;
	}
	if (is_digit(in->peek(in))) {
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
	while (is_digit(in->peek(in))) {
		sb->addc(sb, in->readc(in));
	}
	c = in->peek(in);
	if (c == EOF || is_delimiter(c)) {
		return TKN_NUMBER;
	}
	sprintf_s(error_msg, MAXERROR,
		  "Unexpect char in number (%s...): '%c' (%d)", sb->string(sb),
		  c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
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
