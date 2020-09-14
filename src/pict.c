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
	printf("%lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg", ox, ox + e1x, ox + e2x,
	       ox + e1x + e2x, oy, oy + e1y, oy + e2y, oy + e1y + e2y);
	return;
}

static bool inunit(Floating f)
{
	const Floating zero = 0;
	const Floating one = 1;
	return zero <= f && f <= one;
}

static obj toflt(obj n, Floating *f)
{
	if (!is_number(n)) {
		return error_argument_type(AREA, "vect contains non-number");
	}
	switch (subtype(n)) {
	case NUMBER_FLOATING:
		*f = to_floating(n);
		break;
	case NUMBER_INTEGER:
		*f = (Floating)to_integer(n);
		break;
	default:
		return error_internal(AREA, "BUG! No toflt case for: %d",
				      subtype(n));
	}
	return ok;
}

static bool painted = false;
obj paintp(obj args)
{
	obj rslt;
	Floating ox, oy, e1x, e1y, e2x, e2y;

	painted = true;
	if (is_err(args = chkarity("paintp", 4, args)))
		return args;
	if (is_err(rslt = toflt(caadr(args), &ox)) ||
	    is_err(rslt = toflt(cdadr(args), &oy)) ||
	    is_err(rslt = toflt(caaddr(args), &e1x)) ||
	    is_err(rslt = toflt(cdaddr(args), &e1y)) ||
	    is_err(rslt = toflt(car(cadddr(args)), &e2x)) ||
	    is_err(rslt = toflt(cdr(cadddr(args)), &e2y))) {
		return rslt;
	}
	if (!(inunit(ox) && inunit(ox + e1x) && inunit(ox + e2x) &&
	      inunit(ox + e1x + e2x) && //
	      inunit(oy) && inunit(oy + e1y) && inunit(oy + e2y) &&
	      inunit(oy + e1y + e2y))) {
		return error_argument_value(AREA,
					    "Frame not inside unit square");
	}
	paint(to_bitmap(car(args)), ox, oy, e1x, e1y, e2x, e2y);
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