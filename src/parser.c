#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "pair.h"

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
	switch (tkn->type) {
	case TKN_LIST_OPEN:
		return list(Obj.empty(), port);
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
	obj pair;
static int n = 0;

	struct token *tkn = read_token(port);
printf("list2 tkn-type: %d, value: %s\n", tkn->type, tkn->value);
	switch (tkn->type) {
	case TKN_EOF:
printf("list eof\n");
		eprintf(AREA, "Open list at and of file");
		return error_parser();
	case TKN_LIST_CLOSE:
printf("list close\n");
		return lst;
	default:
if(n > 4) exit(1);
printf("list ... %d\n", ++n);
		pair = cons(parse(tkn, port), Obj.empty());
		if (isnull(lst)) {
printf("list ... head\n");
			lst = pair;
		} else {
printf("list ... rest\n");
			set_cdr(&lst, pair);
		}
printf("car   %ld\n", Obj.toint64(car(lst)));
printf("carp  %ld\n", Obj.toint64(car(pair)));
printf("cadr  %ld\n", Obj.toint64(car(cdr(lst))));
		return list(lst, port);
	}
}
