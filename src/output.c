#include "error.h"
#include "convert.h"
#include "output.h"

#define AREA "OUTPUT"

static void display(struct outport *, obj);
static obj displaystr(obj);

struct outport *defaullt_out = NULL;

static struct outport *dfltout(void)
{
	return defaullt_out == NULL ? (defaullt_out = openout_ptr(stdout)) :
				      defaullt_out;
}

void newline(void)
{
	newlinep(dfltout());
}

void newlinep(struct outport *out)
{
	display(out, Obj.nl());
}

void write(obj dat)
{
	writep(dfltout(), dat);
}

void writep(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, Obj.tostring(str));
	}
}

static void display(struct outport *op, obj dat)
{
	obj str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, Obj.tostring(str));
	}
}

static obj displaystr(obj dat)
{
	if (dat.ispair) {
		eprintf(AREA, "BUG! No displaystr case for pairs!");
		return error_write();
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
