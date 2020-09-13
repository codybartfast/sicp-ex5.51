#include "pict.h"

#include <stdlib.h>
#include "bitmap.h"
#include "error.h"
#include "list.h"
#include "primproc.h"

#define AREA "PICT"

#include <stdio.h>
static void paint(const struct bitmap *bmp, Floating ox, Floating oy,
		  Floating e1x, Floating e1y, Floating e2x, Floating e2y)
{
	printf("Painting ... <bitmap %dx%d> (%lg . %lg) (%lg . %lg) (%lg .%lg) to <bitmap %dx%d>\n",
	       bmp->width, bmp->height, ox, oy, e1x, e1y, e2x, e2y,
	       canvas.width, canvas.height);
	return;
}

static obj toflt(obj n)
{
	if (!is_number(n)) {
		return error_argument_type(AREA, "vect contains non-number");
	}
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		return n;
	case NUMBER_INTEGER:
		return of_floating((Floating)to_integer(n));
	default:
		return error_internal(
			AREA, "BUG! No cnv_to_fltnum case for: %d", subtype(n));
	}
}

static bool painted = false;
obj paintp(obj args)
{
	obj o;
	const struct bitmap *bmp;
	Floating ox, oy, e1x, e1y, e2x, e2y;

	painted = true;
	if (is_err(args = chkarity("paintp", 4, args)))
		return args;
	bmp = to_bitmap(car(args));
	if (is_err(o = toflt(caadr(args))))
		return o;
	ox = to_floating(o);
	if (is_err(o = toflt(cdadr(args))))
		return o;
	oy = to_floating(o);
	if (is_err(o = toflt(caaddr(args))))
		return o;
	e1x = to_floating(o);
	if (is_err(o = toflt(cdaddr(args))))
		return o;
	e1y = to_floating(o);
	if (is_err(o = toflt(car(cadddr(args)))))
		return o;
	e2x = to_floating(o);
	if (is_err(o = toflt(cdr(cadddr(args)))))
		return o;
	e2y = to_floating(o);
	paint(bmp, ox, oy, e1x, e1y, e2x, e2y);
	return _void;
}

obj write_canvas(obj args)
{
	(void)args;
	printf("Displaying canvas ...\n");
	return _void;
}

obj write_canvas_if_painted(obj args)
{
	if (painted) {
		write_canvas(args);
	}
	return _void;
}