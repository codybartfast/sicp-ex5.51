#include "obj.h"
#include "sserror.h"
#include "convert.h"
#include "outport.h"
#include "output.h"

#define AREA "OUTPUT"

static void display(struct outport *, obj *);
static obj *displaystr(obj *);

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
	display(out, nl_object);
}

void write(obj *dat)
{
	writep(dfltout(), dat);
}

void writep(struct outport *op, obj *dat)
{
	obj *str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, str->val.string);
	}
}

static void display(struct outport *op, obj *dat)
{
	obj *str = displaystr(dat);
	if (!iserr(str)) {
		op->writes(op, str->val.string);
	}
}

static obj *displaystr(obj *dat)
{
	switch (dat->type) {
	case TYPE_STRING:
		return dat;
	case TYPE_NUMBER:
		return cnv_number_string(dat);
	default:
		error(AREA, "BUG! tostring unmatched case: %d", dat->type);
		return error_write();
	}
}
