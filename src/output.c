#include "output.h"

#include <stdlib.h>
#include "convert.h"
#include "eval.h"
#include "error.h"
#include "list.h"

#define AREA "OUTPUT"

static obj display(struct outport *, obj);
static obj displaystr(obj);
static obj displaypair(obj);

struct outport *defaullt_out = NULL;

#define MSGSIZE 256
static char msg[MSGSIZE];

static struct outport *dfltout(void)
{
	return defaullt_out == NULL ? (defaullt_out = openout_ptr(stdout)) :
				      defaullt_out;
}

obj newline(void)
{
	return newlinep(dfltout());
}

obj newlinep(struct outport *out)
{
	return display(out, nl);
}

obj write(obj dat)
{
	return writep(dfltout(), dat);
}

obj writep(struct outport *op, obj dat)
{
	obj str = writestr(dat);
	if (is_err(str))
		return str;
	out_writes(op, to_string(str));
	return unspecified;
}

obj writestr(obj dat)
{
	return displaystr(dat);
}

static obj display(struct outport *op, obj dat)
{
	obj str;
	if (is_compound_procedure(dat)) {
		str = displaypair(list4(procedure, procedure_parameters(dat),
					procedure_body(dat),
					of_string("<procedure-env>")));
	} else {
		str = displaystr(dat);
	}
	if (is_err(str))
		return str;
	out_writes(op, to_string(str));
	return unspecified;
}

static obj displaystr(obj dat)
{
	if (is_compound_procedure(dat)) {
		return displaypair(list4(
			of_string("<procedure>"), procedure_parameters(dat),
			procedure_body(dat), of_string("<procedure-env>")));
	}
	if (is_pair(dat)) {
		return displaypair(dat);
	}
	switch (type(dat)) {
	case TYPE_SYMBOL:
		return dat;
	case TYPE_BOOL:
		return cnv_boolean_string(dat);
	case TYPE_NUMBER:
		return cnv_number_string(dat);
	case TYPE_STRING:
		return dat;
	case TYPE_EMPTY_LIST:
		return of_string("()");
	case TYPE_PRIMITIVE_PROCEDURE:
		return of_string("<primitive procedure>");
	case TYPE_ERROR:
		sprintf(msg, "Error-Object, subtype: %d", subtype(dat));
		return of_string(msg);
	default:
		return error_internal(AREA,
				      "BUG! No displaystr case for type: %d",
				      type(dat));
	}
}

static obj displaypair(obj pair)
{
	char *s;
	struct strbldr *sb;

	if ((sb = new_strbldr()) == NULL)
		return error_memory(AREA, "StrBldr");
	sb_addc(sb, '(');

	while (!is_null(pair)) {
		sb_adds(sb, to_string(displaystr(car(pair))));
		if (!is_null(pair = cdr(pair)))
			sb_addc(sb, ' ');
		if (!is_null(pair) && !is_pair(pair)) {
			eprintf(AREA, "Can't handle improper lists");
			exit(1);
		}
	}
	sb_addc(sb, ')');
	s = sb_copy(sb);
	if (s == NULL || sb->errored) {
		return error_memory(AREA, "Copy");
	}
	sb_free(&sb);
	return of_string(s);
}
