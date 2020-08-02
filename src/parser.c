#include <stdlib.h>
#include <string.h>
#include "sserror.h"
#include "lexer.h"
#include "parser.h"

#define AREA "PARSER"

struct in_port *default_in = NULL;
static struct in_port *dfltin(void)
{
	return default_in == NULL ?
		       (default_in = open_input_file_pointer(stdin)) :
		       default_in;
}

static obj *number(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return int64(atoll(tkn->value));
}

obj *read(void)
{
	return readp(dfltin());
}

obj *readp(struct in_port *in)
{
	struct token *tkn = read_token(in);
	switch (tkn->type) {
	case TKN_EOF:
		return lexer_errored ? error_lexor() : eof_object;
	case TKN_NUMBER:
		return number(tkn);
	default:
		error(AREA, "BUG: more token types than cases.");
		return error_internal();
	}
}
