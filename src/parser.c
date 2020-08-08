#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "parser.h"

#define AREA "PARSER"

static obj list(struct token *);
static obj number(struct token *);

struct inport *default_in = NULL;
static struct inport *dfltin(void)
{
	return default_in == NULL ? (default_in = openin_ptr(stdin)) :
				    default_in;
}

obj read(void)
{
	return readp(dfltin());
}

obj readp(struct inport *port)
{
	struct token *tkn = read_token(port);
	switch (tkn->type) {
	case TKN_LIST_OPEN:
		return list(tkn);
	case TKN_NUMBER:
		return number(tkn);
	case TKN_EOF:
		return lexer_errored ? error_lexor() : Obj.eof();
	default:
		eprintf(AREA, "BUG: no parser case for token type: %d",
			tkn->type);
		return error_internal();
	}
}

static obj list(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return Obj.ofint64(atoll(tkn->value));
}

static obj number(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return Obj.ofint64(atoll(tkn->value));
}
