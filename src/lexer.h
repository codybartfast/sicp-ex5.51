#include <stdbool.h>
#include "port.h"
#include "token.h"

bool lexer_errored;
long lexer_error_position;
char *lexer_error_message;

struct token *read_token(struct in_port *);
