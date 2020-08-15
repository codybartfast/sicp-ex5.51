#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "list.h"

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
		return is_err(dat) || is_eof(dat) ? dat : reverse(dat);
	case TKN_EOF:
		return lexer_errored ? error_parser("LEXER", "%s (offset %ld)",
						    lexer_error_message,
						    lexer_error_position) :
				       eof;
	default:
		return error_internal(AREA,
				      "BUG: no parser case for token type: %d",
				      tkn->type);
	}
}

static obj identifier(struct token *tkn)
{
	char *id = malloc((strlen(tkn->value) + 1) * sizeof(char));
	if (id == NULL) {
		eprintf(AREA, "No memory for identifier string");
		return error_memory();
	}
	strcpy(id, tkn->value);
	return of_identifier(id);
}

#define MAX_DIGITS 18
static obj number(struct token *tkn)
{
	char *s = tkn->value;
	size_t maxlen = (*s == '+' || *s == '-') ? MAX_DIGITS + 1 : MAX_DIGITS;
	if (strlen(s) <= maxlen && (strchr(s, '.') == NULL)) {
		return of_integer(atoll(tkn->value));
		// eprintf(AREA, "Number has more than %d digits: %s", MAX_DIGITS,
		// 	tkn->value);
		// return error_parser();
	}
	return of_floating(strtod(tkn->value, NULL));
}

static obj parse_list(obj lst, struct inport *port)
{
	obj fst;

	struct token *tkn = read_token(port);
	switch (tkn->type) {
	case TKN_EOF:
		return error_parser(AREA, "Open list at and of file");
	case TKN_LIST_CLOSE:
		if (is_null(lst)) {
			return error_parser(AREA, "Invalid syntax: \"()\"");
		}
		return lst;
	default:
		fst = parse(tkn, port);
		return is_err(fst) || is_eof(fst) ?
			       fst :
			       parse_list(cons(fst, lst), port);
	}
}

void parser_freetemp(void)
{
	lexer_freetemp();
}
