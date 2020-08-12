#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "list.h"
#include "windows.h"

#define AREA "PARSER"

static obj parse(struct token *tkn, struct inport *port);
static obj parse_list(obj lst, struct inport *port);
static obj identifier(struct token *tkn);
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
	case TKN_IDENTIFIER:
		return identifier(tkn);
	case TKN_NUMBER:
		return number(tkn);
	case TKN_LIST_OPEN:
		dat = parse_list(emptylst, port);
		return iserr(dat) || iseof(dat) ? dat : reverse(dat);
	case TKN_EOF:
		return lexer_errored ? error_lexor() : Obj.eof();
	default:
		eprintf(AREA, "BUG: no parser case for token type: %d",
			tkn->type);
		return error_internal();
	}
}

static obj identifier(struct token *tkn)
{
	size_t idlen = strlen(tkn->value) + 1;
	char *id = malloc((idlen + 1) * sizeof(char));
	if (id == NULL) {
		eprintf(AREA, "No memory for identifier string");
		return error_memory();
	}
	strcpy_s(id, idlen, tkn->value);
	return Obj.ofidentifier(id);
}

#define MAX_DIGITS 18
static obj number(struct token *tkn)
{
	char c = *tkn->value;
	size_t maxlen = (c == '+' || c == '-') ? MAX_DIGITS + 1 : MAX_DIGITS;
	if (strlen(tkn->value) > maxlen) {
		eprintf(AREA, "Number has more than %d digits: %s", MAX_DIGITS,
			tkn->value);
		return error_parser();
	}
	return Obj.ofint64(atoll(tkn->value));
}

static obj parse_list(obj lst, struct inport *port)
{
	obj fst;

	struct token *tkn = read_token(port);
	switch (tkn->type) {
	case TKN_EOF:
		eprintf(AREA, "Open list at and of file");
		return error_parser();
	case TKN_LIST_CLOSE:
		if (isnull(lst)) {
			eprintf(AREA, "Invalid syntax: \"()\"");
			return error_parser();
		}
		return lst;
	default:
		fst = parse(tkn, port);
		return iserr(fst) || iseof(fst) ?
			       fst :
			       parse_list(cons(fst, lst), port);
	}
}

void parser_freetemp(void)
{
	lexer_freetemp();
}
