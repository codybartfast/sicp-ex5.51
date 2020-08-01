#ifndef LEXER_H
#define LEXER_H 1

#include <stdbool.h>
#include "port.h"
#include "token.h"

extern bool lexer_errored;
extern long lexer_error_position;
extern char *lexer_error_message;

struct token *read_token(struct in_port *);

#endif
