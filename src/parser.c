#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lexer.h"
#include "list.h"
#include "primproc.h"

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

obj read_p(obj args)
{
	if (is_err(args = chkarity("read", 0, args)))
		return args;
	return read();
}

static obj check_eof(void)
{
	return lexer_errored ? error_parser("LEXER", "%s (ln:%ld, col:%ld)",
					    lexer_error_message,
					    lexer_error_location.line + 1,
					    lexer_error_location.column) :
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

static obj string(struct token *tkn)
{
	char *str = token_string(tkn);
	if (str == NULL)
		return error_memory(AREA, "String");
	return of_string(str);
}

static obj mark_to_list(struct inport *port, obj keyword, char *msg)
{
	obj dat = readp(port);
	if (is_err(dat))
		return dat;
	switch (type(dat)) {
	case TYPE_BOOL:
	case TYPE_EMPTY_LIST:
	case TYPE_NUMBER:
	case TYPE_PAIRPTR:
	case TYPE_STRING:
	case TYPE_SYMBOL:
		return list2(keyword, dat);
	default:
		return error_parser(AREA, msg, errstr(dat));
	}
}

static obj quote_(struct inport *port)
{
	return mark_to_list(
		port, quote,
		"Expected a datum after \"'\" (quote), but got \"%s\"");
}

static obj quasiquote_(struct inport *port)
{
	return mark_to_list(
		port, quasiquote,
		"Expected a datum after '`' (quasiquote), but got \"%s\"");
}

static obj unquote_(struct inport *port)
{
	return mark_to_list(
		port, unquote,
		"Expected a datum after ',' (unquote), but got \"%s\"");
}

static obj parse_list(obj, struct inport *);
static obj parse(struct token *tkn, struct inport *port)
{
	switch (tkn->type) {
	case TKN_IDENTIFIER:
		return identifier(tkn);
	case TKN_NUMBER:
		return number(tkn);
	case TKN_LIST_OPEN:
		return parse_list(emptylst, port);
	case TKN_LIST_CLOSE:
		return error_parser(AREA, "unexpected: ')'");
	case TKN_STRING:
		return string(tkn);
	case TKN_QUOTE:
		return quote_(port);
	case TKN_QUASIQUOTE:
		return quasiquote_(port);
	case TKN_UNQUOTE:
		return unquote_(port);
	case TKN_EOF:
		return check_eof();
	default:
		return error_internal(AREA,
				      "BUG: no parser case for token type: %d",
				      tkn->type);
	}
}

static obj dot(obj lst, struct inport *port)
{
	obj dat;
	struct token *tkn;

	if ((tkn = read_token(port))->type == TKN_IDENTIFIER) {
		obj id = identifier(tkn);
		if (read_token(port)->type == TKN_LIST_CLOSE) {
			if (is_null(lst) || !is_symbol(car(lst))) {
				return error_parser(AREA,
						    "'.' (dot) is unexpected");
			}
			for (dat = lst = reverse(lst); is_pair(cdr(dat));
			     dat = cdr(dat))
				;
			set_cdr(dat, id);
			return lst;
		}
	}
	return error_parser(AREA, "expected one identifier after '.' (dot)");
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
			return error_parser(AREA, "Open list at end of input");
	case TKN_LIST_CLOSE:
		return reverse(lst);
	case TKN_DOT:
		return dot(lst, port);
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
