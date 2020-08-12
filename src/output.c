#include "output.h"

#include <stdlib.h>
#include "error.h"
#include "convert.h"
#include "windows.h"

#define AREA "OUTPUT"

static obj display(struct outport *, obj);
static obj displaystr(obj);
static obj displaypair(obj);

struct outport *defaullt_out = NULL;

#define MSGSIZE (1 << 8)
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
	return display(out, Obj.nl());
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
	op->writes(op, Obj.to_string(str));
	return Obj.unspecified();
}

char *debugstr(obj dat)
{
	return Obj.to_string(writestr(dat));
}

obj writestr(obj dat)
{
	return displaystr(dat);
}

static obj display(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (is_err(str))
		return str;
	op->writes(op, Obj.to_string(str));
	return Obj.unspecified();
}

static obj displaystr(obj dat)
{
	if (is_pair(dat)) {
		return displaypair(dat);
	}
	switch (type(dat)) {
	case TYPE_SYMBOL:
		return dat;
	case TYPE_STRING:
		return dat;
	case TYPE_NUMBER:
		return cnv_number_string(dat);
	case TYPE_EMPTY_LIST:
		return Obj.of_string("()");
	case TYPE_PRIMITIVE_PROCEDURE:
		return Obj.of_string("<primitive procedure>");
	case TYPE_ERROR:
		sprintf_s(msg, MSGSIZE, "Error, subtype: %d", subtype(dat));
		return Obj.of_string(msg);
	default:
		eprintf(AREA, "BUG! No displaystr case for type: %d",
			type(dat));
		return error_write();
	}
}

static obj displaypair(obj pair)
{
	char *s;
	struct strbldr *sb;

	if ((sb = new_strbldr()) == NULL)
		return error_memory();
	sb->addc(sb, '(');

	while (!is_null(pair)) {
		sb->adds(sb, Obj.to_string(displaystr(car(pair))));
		if (!is_null(pair = cdr(pair)))
			sb->addc(sb, ' ');
		if (!is_null(pair) && !is_pair(pair)) {
			eprintf(AREA, "Can't handle improper lists");
			exit(1);
		}
	}
	sb->addc(sb, ')');
	s = sb->copy(sb);
	if (s == NULL || sb->errored) {
		return error_memory();
	}
	sb->free(&sb);
	return Obj.of_string(s);
}
