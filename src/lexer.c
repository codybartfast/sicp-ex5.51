#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "character.h"
#include "strbldr.h"
#include "error.h"

#define AREA "LEXER"
#define MAXERROR 256
#define MSGLIM "64"

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct inport *);
static enum token_type identifier(char c, struct inport *);
static enum token_type number(char c, struct inport *);
static enum token_type peculiar(char c, struct inport *in);
static enum token_type comment(char c, struct inport *in);
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
		sb_clear(sb);
	}
	token.type = scan(in);
	token.value = sb_string(sb);
	if (sb->errored) {
		token.type = TKN_EOF;
		lexer_error(in->read_count, "strbldr (memory) error");
	}
	return &token;
}

static enum token_type scan(struct inport *in)
{
	int c;

	while (is_whitespace(in_peek(in)))
		in_readc(in);
	if ((c = in_readc(in)) == EOF) {
		return TKN_EOF;
	}
	token.position = in->read_count;
	if (is_initial(c))
		return identifier(c, in);
	if (c == '(') {
		sb_addc(sb, '(');
		return TKN_LIST_OPEN;
	}
	if (c == ')') {
		sb_addc(sb, ')');
		return TKN_LIST_CLOSE;
	}
	if (is_peculiar_identifier(c)) {
		return peculiar(c, in);
	}
	if (is_digit(c) || c == '.') {
		return number(c, in);
	}
	if (c == ';') {
		return comment(';', in);
	}
	sprintf(error_msg, "Unexpected start of datum: '%c' (0x%0X)", c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

static enum token_type identifier(char c, struct inport *in)
{
	sb_addc(sb, c);
	while (is_subsequent(c = in_peek(in))) {
		sb_addc(sb, in_readc(in));
	}
	if (c == EOF || is_delimiter(c)) {
		return TKN_IDENTIFIER;
	}
	sprintf(error_msg,
		"Unexpected char in identifier starting '%." MSGLIM
		"s': %c' (0x%0X)",
		sb_string(sb), c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

static enum token_type peculiar(char c, struct inport *in)
{
	if (is_delimiter(in_peek(in))) {
		sb_addc(sb, c);
		return TKN_IDENTIFIER;
	}
	if (is_digit(in_peek(in))) {
		return number(c, in);
	}
	char p = in_peek(in);
	sprintf(error_msg,
		"Unexpected char following initial '%c': '%c' (0x%0X)", c, p,
		p);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

static enum token_type number(char c, struct inport *in)
{
	int prdcnt = 0;
	int lastc = -1;

	sb_addc(sb, c);
	while (is_digit(c = in_peek(in)) || c == '.') {
		prdcnt += (c == '.');
		sb_addc(sb, in_readc(in));
		lastc = c;
	}
	if (prdcnt > 1 || lastc == '.') {
		sprintf(error_msg, "Invalid number: '%." MSGLIM "s'",
			sb_string(sb));
		lexer_error(in->read_count, error_msg);
		return EOF;
	}
	if (c == EOF || is_delimiter(c)) {
		return TKN_NUMBER;
	}
	sprintf(error_msg,
		"Unexpected char in number starting '%." MSGLIM
		"s': '%c' (0x%0X)",
		sb_string(sb), c, c);
	lexer_error(in->read_count, error_msg);
	return EOF;
}

static enum token_type comment(char c, struct inport *in)
{
	sb_addc(sb, c);
	while ((c = in_readc(in)) != '\n' && c != '\r' && c != EOF)
		sb_addc(sb, c);
	return TKN_COMMENT;
}

void lexer_freetemp(void)
{
	sb_free(&sb);
}

static void lexer_error(long position, char *msg)
{
	lexer_errored = true;
	lexer_error_position = position;
	lexer_error_message = msg;
}
