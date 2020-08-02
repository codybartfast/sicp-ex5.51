#include "obj.h"
#include "sserror.h"
#include "convert.h"
#include "outport.h"
#include "output.h"

#define AREA "OUTPUT"

static void display(struct out_port *op, obj *ob);
static obj *asstring(obj *obj);

struct out_port *defaullt_out = NULL;

static struct out_port *dfltout(void)
{
	return defaullt_out == NULL ?
		       (defaullt_out = open_output_file_pointer(stdout)) :
		       defaullt_out;
}

void newline(void){
	newlinep(dfltout());
}

void newlinep(struct out_port *op)
{
	display(op, nl_object);
}

void write(obj *obj)
{
	writep(dfltout(), obj);
}

void writep(struct out_port *op, obj *ob)
{
	display(op, ob);
}

static void display(struct out_port *op, obj *ob)
{
	obj *asstr = asstring(ob);
	if (!iserr(asstr)) {
		op->writes(op, asstr->val.string);
	}
}

static obj *asstring(obj *obj)
{
	switch (obj->type) {
	case TYPE_STRING:
		return obj;
	case TYPE_NUMBER:
		return cnv_number_string(obj);
	default:
		error(AREA, "more types than cases");
		return error_write();
	}
}
