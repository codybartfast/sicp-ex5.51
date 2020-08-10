#include <stdlib.h>
#include "error.h"
#include "convert.h"
#include "output.h"

#define AREA "OUTPUT"

static obj display(struct outport *, obj);
static obj displaystr(obj);
static obj displaypair(obj);

struct outport *defaullt_out = NULL;

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
	obj str = displaystr(dat);
	if (iserr(str))
		return str;
	op->writes(op, Obj.tostring(str));
	return Obj.unspecified();
}

// Should return unpsecified/error
static obj display(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (iserr(str))
		return str;
	op->writes(op, Obj.tostring(str));
	return Obj.unspecified();
}

static obj displaystr(obj dat)
{
	if (ispair(dat)) {
		return displaypair(dat);
	} else {
		switch (dat.simp.type) {
		case TYPE_STRING:
			return dat;
		case TYPE_NUMBER:
			return cnv_number_string(dat);
		default:
			eprintf(AREA, "BUG! No displaystr case for type: %d",
				dat.simp.type);
			return error_write();
		}
	}
}

static obj displaypair(obj pair)
{
	char *s;
	struct strbldr *sb;

	if ((sb = new_strbldr()) == NULL)
		return error_memory();
	sb->addc(sb, '(');

	while (!isnull(pair)) {
		sb->adds(sb, Obj.tostring(displaystr(car(pair))));
		if (!isnull(pair = cdr(pair)))
			sb->addc(sb, ' ');
		if (!isnull(pair) && !ispair(pair)) {
			eprintf(AREA, "Can't handle whatsit lists");
			exit(1);
		}
	}
	sb->addc(sb, ')');
	s = sb->copy(sb);
	if (s == NULL || sb->errored) {
		return error_memory();
	}
	sb->free(&sb);
	return Obj.ofstring(s);
}
