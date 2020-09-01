#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "list.h"

#define AREA "PARSER"

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

static obj check_eof(void)
{
	return lexer_errored ?
		       error_parser("LEXER", "%s (offset %ld)",
				    lexer_error_message, lexer_error_position) :
		       eof;
}

static char *token_string(struct token *tkn)
{
	char *str = malloc((strlen(tkn->value) + 1) * sizeof(char));
	if (str == NULL) {
		return NULL;
	}
	strcpy(str, tkn->value);
	return str;
}

static obj identifier(struct token *tkn)
{
	char *id = token_string(tkn);
	if (id == NULL)
		return error_memory(AREA, "Identifier");
	return of_identifier(id);
}

static obj string(struct token *tkn)
{
	char *str = token_string(tkn);
	if (str == NULL)
		return error_memory(AREA, "String");
	return of_string(str);
}

#define MAX_DIGITS 18
static obj number(struct token *tkn)
{
	char *s = tkn->value;
	size_t maxlen = (*s == '+' || *s == '-') ? MAX_DIGITS + 1 : MAX_DIGITS;
	if (strlen(s) <= maxlen && (strchr(s, '.') == NULL)) {
		return of_integer(atoll(tkn->value));
	} else {
		return of_floating(strtold(tkn->value, NULL));
	}
}

static obj parse_list(obj, struct inport *);
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
	case TKN_LIST_CLOSE:
		return error_parser(AREA, "unexpected: ')'");
	case TKN_STRING:
		return string(tkn);
	case TKN_EOF:
		return check_eof();
	default:
		return error_internal(AREA,
				      "BUG: no parser case for token type: %d",
				      tkn->type);
	}
}

static obj parse_list(obj lst, struct inport *port)
{
	obj dat;

	struct token *tkn;
	while ((tkn = read_token(port))->type == TKN_COMMENT)
		;
	switch (tkn->type) {
	case TKN_EOF:
		if (is_err(dat = check_eof()))
			return dat;
		else
			return error_parser(AREA, "Open list at and of file");
	case TKN_LIST_CLOSE:
		if (is_null(lst)) {
			return error_parser(AREA, "Invalid syntax: \"()\"");
		}
		return lst;
	default:
		dat = parse(tkn, port);
		if (is_err(dat) || is_eof(dat))
			return dat;
		lst = cons(dat, lst);
		return (is_err(lst)) ? lst : parse_list(lst, port);
	}
}

obj readp(struct inport *port)
{
	struct token *tkn;
	while ((tkn = read_token(port))->type == TKN_COMMENT)
		;
	if (tkn->type == TKN_EOF)
		return check_eof();
	return parse(tkn, port);
}

void parser_freetemp(void)
{
	lexer_freetemp();
}
