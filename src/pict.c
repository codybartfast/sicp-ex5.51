#include "pict.h"

#include <math.h>
#include <stdlib.h>
#include "bitmap.h"
#include "environment.h"
#include "error.h"
#include "list.h"
#include "output.h"
#include "primproc.h"
#include "strbldr.h"

#define AREA "PICT"

static void clear_canvas(void)
{
	int x, y;

	for (y = 0; y < CANVAS_HEIGHT; y++) {
		for (x = 0; x < CANVAS_WIDTH; x++) {
			canvas.data[x + (y * CANVAS_WIDTH)] = 255;
		}
	}
}

static int breakline(Floating xprop, Floating yprop, Floating *xinc,
		     Floating *yinc)
{
	int steps =
		(int)(fabs(xprop * CANVAS_WIDTH) + fabs(yprop * CANVAS_HEIGHT));
	*xinc = steps ? ((Floating)(CANVAS_WIDTH * xprop)) / (Floating)steps :
			0;
	*yinc = steps ? ((Floating)(CANVAS_HEIGHT * yprop)) / (Floating)steps :
			0;
	return steps;
}

#include <stdio.h>
static void paint_u(const struct bitmap *bmp, Floating ox, Floating oy,
		    Floating e1x, Floating e1y, Floating e2x, Floating e2y)
{
	// printf("Painting ... <bitmap %dx%d> (%lg . %lg) (%lg . %lg) (%lg .%lg) to <bitmap %dx%d>\n",
	//        bmp->width, bmp->height, ox, oy, e1x, e1y, e2x, e2y,
	//        canvas.width, canvas.height);
	// printf("%lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg\n", ox, ox + e1x,
	//        ox + e2x, ox + e1x + e2x, oy, oy + e1y, oy + e2y,
	//        oy + e1y + e2y);

	int cwd = CANVAS_WIDTH;
	int cht = CANVAS_HEIGHT;
	int bwd = bmp->width;
	int bht = bmp->height;

	Floating e1xinc;
	Floating e1yinc;
	int e1steps = breakline(e1x, e1y, &e1xinc, &e1yinc);
	Floating e2xinc;
	Floating e2yinc;
	int e2steps = breakline(e2x, e2y, &e2xinc, &e2yinc);

	Floating bxinc = e1steps ? ((Floating)bwd) / (Floating)e1steps : 0;
	Floating byinc = e2steps ? ((Floating)bht) / (Floating)e2steps : 0;

	int offx = (int)(ox * (cwd - 0));
	int offy = (int)(oy * (cht - 0));
	int i1, i2;

	for (i2 = 0; i2 < e2steps; i2++) {
		for (i1 = 0; i1 < e1steps; i1++) {
			int bx = (int)(bxinc * i1);
			int by = (int)(bht - 1 - (byinc * i2));
			unsigned char val = bmp->data[bx + (by * bwd)];
			int cx = (int)(offx + (e1xinc * i1) + (e2xinc * i2));
			int cy = (int)(cht - 1 -
				       (offy + (e1yinc * i1) + (e2yinc * i2)));
			if (cx >= canvas.width)
				printf("Too Fat! - %d\n", cx);
			if (cx < 0)
				printf("Too Skinny! - %d\n", cx);
			if (cy >= canvas.height)
				printf("Too Tall! - %d\n", cy);
			if (cy < 0)
				printf("Too Short! - %d\n", cy);
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

static bool unwritenpaint = false;
obj paint(obj args)
{
	obj rslt;
	Floating ox, oy, e1x, e1y, e2x, e2y;

	if (!unwritenpaint) {
		clear_canvas();
	}
	if (is_err(args = chkarity("%paint", 2, args)))
		return args;
	obj frame = cadr(args);
	if (is_err(rslt = toflt(caar(frame), &ox)) ||
	    is_err(rslt = toflt(cdar(frame), &oy)) ||
	    is_err(rslt = toflt(caadr(frame), &e1x)) ||
	    is_err(rslt = toflt(cdadr(frame), &e1y)) ||
	    is_err(rslt = toflt(caddr(frame), &e2x)) ||
	    is_err(rslt = toflt(cdddr(frame), &e2y))) {
		return rslt;
	}
	if (!(inunit(ox) && inunit(ox + e1x) && inunit(ox + e2x) &&
	      inunit(ox + e1x + e2x) && //
	      inunit(oy) && inunit(oy + e1y) && inunit(oy + e2y) &&
	      inunit(oy + e1y + e2y))) {
		return error_argument_value(AREA,
					    "Frame not inside unit square");
	}
	paint_u(to_bitmap(car(args)), ox, oy, e1x, e1y, e2x, e2y);
	unwritenpaint = true;
	return _void;
}

obj draw_line(obj args)
{
	obj rslt;
	Floating x1, y1, x2, y2;

	if (!unwritenpaint) {
		clear_canvas();
	}
	unwritenpaint = true;
	if (is_err(args = chkarity("draw-line", 2, args)))
		return args;
	if (is_err(rslt = toflt(caar(args), &x1)) ||
	    is_err(rslt = toflt(cdar(args), &y1)) ||
	    is_err(rslt = toflt(caadr(args), &x2)) ||
	    is_err(rslt = toflt(cdadr(args), &y2))) {
		return rslt;
	}
	int i;
	Floating xinc;
	Floating yinc;
	int steps = breakline(x2 - x1, y2 - y1, &xinc, &yinc);
	for (i = 0; i < steps; i++) {
		int x = (int)(x1 * (CANVAS_WIDTH - 1)) + (i * xinc);
		int y = CANVAS_HEIGHT - 1 -
			(int)((y1 * (CANVAS_HEIGHT - 1)) + (i * yinc));
		// printf("drawing %d, %d, i:%d, inc:%lg\n", x, y, i, xinc);
		canvas.data[x + (y * CANVAS_WIDTH)] = 0;
	}
	return _void;
}

obj write_canvas(obj args)
{
	(void)args;
	struct strbldr *sb = new_strbldr();
	if (sb == NULL)
		return error_memory(AREA, "making strbldr");
	sb_adds(sb, to_string(lookup_variable_value(of_identifier("%pict-path"),
						    tge())));
	sb_adds(sb, ".png");
	if (sb->errored)
		return error_memory(AREA, "using strbldr");
	char *path = sb_string(sb);
	displaydat(of_string("Writing picture to '"));
	displaydat(of_string(path));
	displaydat(of_string("'."));
	newline(emptylst);
	writebmp(&canvas, path);
	unwritenpaint = false;
	return _void;
}

obj write_canvas_if_painted(obj args)
{
	if (unwritenpaint) {
		write_canvas(args);
	}
	return _void;
}
