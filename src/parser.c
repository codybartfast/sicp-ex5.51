#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "lexer.h"
#include "parser.h"

#define AREA "PARSER"

static obj *number(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return int64(atoll(tkn->value));
}

obj *read(struct in_port *in)
{
	struct token *tkn = read_token(in);
	switch (tkn->type) {
	case TKN_EOF:
		return lexer_errored ? error_lexor() : eof;
	case TKN_NUMBER:
		return number(tkn);
	default:
		error(AREA, "BUG: more token types than cases.");
		return error_internal();
	}
}
