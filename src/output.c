#include "output.h"

#include <stdlib.h>
#include "convert.h"
#include "mceval.h"
#include "error.h"
#include "list.h"
#include "primproc.h"

#define AREA "OUTPUT"

static obj displaypair(struct outport *, obj, bool);

struct outport *defaullt_out = NULL;

#define MSGSIZE 256
static char msg[MSGSIZE];

struct outport *default_out(void)
{
	return defaullt_out == NULL ? (defaullt_out = openout_ptr(stdout)) :
				      defaullt_out;
}

obj newline(obj args)
{
	if (is_err(args = chkarity("newline", 0, args)))
		return args;
	return newlinep(default_out());
}

obj newlinep(struct outport *out)
{
	return displayp(out, nl);
}

obj display(obj args)
{
	if (is_err(args = chkarity("display", 1, args)))
		return args;
	return displaydat(car(args));
}

obj displaydat(obj dat)
{
	return displayp(default_out(), dat);
}

obj write(obj dat)
{
	return writep(default_out(), dat);
}

obj writestr(obj dat)
{
	struct outport *op = openout_string();
	if (op == NULL)
		return error_memory(AREA, "writestr");
	writep(op, dat);
	return of_string(sb_string(op->sb));
}

obj writep(struct outport *op, obj dat)
{
	switch (type(dat)) {
	case TYPE_STRING:
		out_writec(op, '"');
		out_writes(op, to_string(dat));
		out_writec(op, '"');
		return _void;
	default:
		return displayp(op, dat);
	}
}

obj displayp(struct outport *op, obj dat)
{
	if (is_pair(dat)) {
		return displaypair(op, dat, true);
	}
	switch (type(dat)) {
	case TYPE_SYMBOL:
		out_writes(op, dat.val.string);
		return _void;
	case TYPE_BOOL:
		return cnv_boolean_string(op, dat);
	case TYPE_NUMBER:
		return cnv_number_string(op, dat);
	case TYPE_STRING:
		out_writes(op, dat.val.string);
		return _void;
	case TYPE_EMPTY_LIST:
		out_writes(op, "()");
		return _void;
	case TYPE_PRIMITIVE_PROCEDURE:
		out_writes(op, "<primitive procedure>");
		return _void;
	case TYPE_UNSPECIFIED:
		out_writes(op, "<unspecified>");
		return _void;
	case TYPE_VOID:
		return _void;
	case TYPE_BROKEN_HEART:
		out_writes(op, "<Broken Heart!>");
		return _void;
	case TYPE_BITMAP:
		return cnv_bitmap_string(op, dat);
	case TYPE_ERROR:
		sprintf(msg, "Error-Object, subtype: %d", subtype(dat));
		out_writes(op, msg);
		return _void;
	default:
		return error_internal(
			AREA, "BUG! No displaysb case for type: %d", type(dat));
	}
}

static obj displaypair(struct outport *op, obj pair, bool first)
{
	if (is_compound_procedure(pair)) {
		if (length_u(pair) != 4) {
			return error_argument_type(
				AREA, "Expected procedure length of 4, got %d",
				length_u(pair));
		}
		return displaypair(op,
				   list4(of_string("<procedure>"),
					 procedure_parameters(pair),
					 procedure_body(pair),
					 of_string("<procedure-env>")),
				   first);
	}

	if (is_null(pair)) {
		out_writes(op, ")");
		return _void;
	}

	if (first)
		out_writec(op, '(');

	displayp(op, car(pair));

	obj nxt = cdr(pair);
	if (is_pair(nxt)) {
		out_writec(op, ' ');
		displaypair(op, nxt, false);
	} else if (!is_null(nxt)) {
		out_writes(op, " . ");
		displayp(op, nxt);
	}
	if (first) {
		out_writec(op, ')');
	}
	return _void;
}

obj void_p(obj args)
{
	(void)args;
	return _void;
}
