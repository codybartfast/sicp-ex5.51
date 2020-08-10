#ifndef LEXER_H
#define LEXER_H 1

#include <stdbool.h>
#include "inport.h"

enum token_type { TKN_EOF = -1, TKN_NUMBER = 1, TKN_LIST_OPEN, TKN_LIST_CLOSE };

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
