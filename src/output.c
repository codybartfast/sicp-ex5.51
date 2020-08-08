#include <stdlib.h>
#include "error.h"
#include "convert.h"
#include "output.h"
#include "pair.h"

#define AREA "OUTPUT"

static void display(struct outport *, obj);
static obj displaystr(obj);
static obj displaypair(obj);

struct outport *defaullt_out = NULL;

static struct outport *dfltout(void)
{
	return defaullt_out == NULL ? (defaullt_out = openout_ptr(stdout)) :
				      defaullt_out;
}

// Should return unpsecified/error
void newline(void)
{
	newlinep(dfltout());
}

// Should return unpsecified/error
void newlinep(struct outport *out)
{
	display(out, Obj.nl());
}

// Should return unpsecified/error
void write(obj dat)
{
	writep(dfltout(), dat);
}

// Should return unpsecified/error
void writep(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, Obj.tostring(str));
	}
}

// Should return unpsecified/error
static void display(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, Obj.tostring(str));
	}
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
	struct strbldr *sb = new_strbldr();
	if (sb == NULL)
		return error_memory();
	sb->addc(sb, '(');

	while (!isnull(pair)) {
		//sb->adds(sb, "blah");
		sb->adds(sb, Obj.tostring(displaystr(car(pair))));
		if (!isnull(pair = cdr(pair)))
			sb->addc(sb, ' ');
		if (!isnull(pair) && !ispair(pair)) {
			eprintf(AREA, "Can't handle whatsit lists");
			exit(1);
		}
	}
	sb->addc(sb, ')');
	// Need to check copy result ///////////////////
	obj rslt = Obj.ofstring(sb->copy(sb));
	printf("display sending: %s\n", sb->copy(sb));
	return rslt;
}
