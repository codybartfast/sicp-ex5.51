#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "list.h"
#include "parser.h"

#define AREA "PARSER"

static obj parse(struct token *tkn, struct inport *port);
static obj list(obj lst, struct inport *port);
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
	return parse(tkn, port);
}

static obj parse(struct token *tkn, struct inport *port)
{
	obj dat;

	switch (tkn->type) {
	case TKN_LIST_OPEN:
		dat = list(Obj.empty(), port);
		return iserr(dat) || Obj.iseof(dat) ? dat : reverse(dat);
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

static obj number(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return Obj.ofint64(atoll(tkn->value));
}

static obj list(obj lst, struct inport *port)
{
	obj fst;

	struct token *tkn = read_token(port);
	switch (tkn->type) {
	case TKN_EOF:
		eprintf(AREA, "Open list at and of file");
		return error_parser();
	case TKN_LIST_CLOSE:
		return lst;
	default:
		fst = parse(tkn, port);
		return iserr(fst) || Obj.iseof(fst) ?
			       fst :
			       list(cons(fst, lst), port);
	}
}
