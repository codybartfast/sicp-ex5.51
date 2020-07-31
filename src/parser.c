#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "lexer.h"
#include "parser.h"

#define AREA "PARSER"

obj *number(struct token *, obj *);

obj *read(struct in_port *in)
{
	struct token *tkn;
	obj *obj;
	if (iserr(obj = new_obj()))
		return obj;
	tkn = read_token(in);
	switch (tkn->type) {
	case TKN_EOF:
		if (lexer_errored)
			return error_lexor();
		else
			return eof;
	case TKN_NUMBER:
		return number(tkn, obj);
	default:
		error(AREA, "BUG: more token types than cases.");
		return error_internal();
	}
}

obj *number(struct token *tkn, obj *obj)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return int64_obj(atoll(tkn->value), obj);
}
