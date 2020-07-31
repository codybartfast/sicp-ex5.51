#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"

#define AREA "LEXER"
#define error_len 100
#define INITIAL_BUFFSIZE (1 << 2)

bool lexer_errored = false;
long lexer_error_position = -1;
char *lexer_error_message = NULL;

static enum token_type scan(struct in_port *);
static enum token_type number(struct in_port *);
static bool peek_delimited(struct in_port *in);
static enum token_type add(int c);
static int grow_buff(void);
static void lexical_error(long position, char *msg);

static int buffidx, buffsize = INITIAL_BUFFSIZE;
static int maxlen = INITIAL_BUFFSIZE - 1;
static char *buff = NULL;
static struct token token;
static char error_msg[error_len];

struct token *read_token(struct in_port *in)
{
	if (lexer_errored != false) {
		lexer_errored = false;
		lexer_error_position = -1;
		lexer_error_message = NULL;
	}
	if (buff == NULL) {
		buff = (char *)malloc(buffsize * sizeof(char));
		if (buff == NULL) {
			error(AREA, "No memory for buff");
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
	while (isdigit(peek_char(in))) {
		if (add(read_char(in)) == TKN_EOF)
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

enum token_type add(int c)
{
	if (buffidx >= maxlen) {
		if (grow_buff() != 0)
			return TKN_EOF;
	}
	return *(buff + buffidx++) = c;
}

int grow_buff(void)
{
	int i;
	char *old_buff = buff;

	buffsize *= 2;
	maxlen = buffsize - 1;
	buff = (char *)malloc(buffsize * sizeof(char));
	if (buff == NULL) {
		free(old_buff);
		return -1;
	}
	for (i = 0; i < buffidx; i++)
		*(buff + i) = *(old_buff + i);
	token.value = buff;
	free(old_buff);
	return 0;
}

void lexical_error(long position, char *msg)
{
	lexer_errored = true;
	lexer_error_position = position;
	lexer_error_message = msg;
	error(AREA, msg);
}
