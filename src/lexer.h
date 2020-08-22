#ifndef LEXER_H
#define LEXER_H
#include "sicpstd.h"

#include <stdbool.h>
#include "inport.h"

enum token_type {
	TKN_EOF = -1,
	TKN_COMMENT = 1,
	TKN_IDENTIFIER, // 2
	TKN_LIST_OPEN, // 3
	TKN_LIST_CLOSE, // 4
	TKN_NUMBER, //5
	TKN_STRING, //6
};

struct token {
	enum token_type type;
	char *value;
	long position;
};

extern bool lexer_errored;
extern long lexer_error_position;
extern char *lexer_error_message;

struct token *read_token(struct inport *);
void lexer_freetemp(void);

#endif
