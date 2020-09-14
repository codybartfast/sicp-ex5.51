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
	printf("%lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg\n", ox, ox + e1x,
	       ox + e2x, ox + e1x + e2x, oy, oy + e1y, oy + e2y,
	       oy + e1y + e2y);

	int cwd = CANVAS_WIDTH;
	int cht = CANVAS_HEIGHT;
	int bwd = bmp->width;
	int bht = bmp->height;

	int e1steps = abs(e1x * cwd) + abs(e1y * cwd);
	int e2steps = abs(e2x * cht) + abs(e2y * cht);
	Floating e1xinc =
		e1steps ? ((Floating)(cwd * e1x)) / (Floating)e1steps : 0;
	Floating e1yinc =
		e1steps ? ((Floating)(cht * e1y)) / (Floating)e1steps : 0;
	Floating e2xinc =
		e2steps ? ((Floating)(cwd * e2x)) / (Floating)e2steps : 0;
	Floating e2yinc =
		e2steps ? ((Floating)(cht * e2y)) / (Floating)e2steps : 0;
	Floating bxinc = e1steps ? ((Floating)bwd) / (Floating)e1steps : 0;
	Floating byinc = e2steps ? ((Floating)bht) / (Floating)e2steps : 0;

	int offx = ox * cwd;
	int offy = oy * cht;
	int i1, i2;

	for (i2 = 0; i2 < e2steps; i2++) {
		for (i1 = 0; i1 < e1steps; i1++) {
			int bx = bxinc * i1;
			int by = bht - (byinc * i2);
			unsigned char val = bmp->data[bx + (by * bwd)];
			int cx = offx + (e1xinc * i1) + (e2xinc * i2);
			int cy = cht - (offy + (e1yinc * i1) + (e2yinc * i2));
			canvas.data[cx + (cy * cwd)] = val;
		}
	}
	return;
}

static bool inunit(Floating f)
{
	return 0 <= f && f <= 1;
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
	writebmp(&canvas);
	return _void;
}

obj write_canvas_if_painted(obj args)
{
	if (painted) {
		write_canvas(args);
	}
	return _void;
}