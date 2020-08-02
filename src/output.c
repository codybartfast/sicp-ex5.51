#include "obj.h"
#include "sserror.h"
#include "convert.h"
#include "out_port.h"
#include "output.h"

#define AREA "write"

static void display(struct out_port *op, obj *ob);
static obj *asstring(obj *obj);

void newline(struct out_port *op){
	display(op, nl_object);
}

void write(struct out_port *op, obj *ob)
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
