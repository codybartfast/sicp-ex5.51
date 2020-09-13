#include "pict.h"

#include <stdlib.h>
#include "error.h"
#include "primproc.h"

// static struct bitmap *canvas = NULL;
static bool painted = false;

#include <stdio.h>
obj paintp(obj args)
{
	if (is_err(args = chkarity("paintp", 4, args)))
		return args;
	painted = true;
	printf("Painting ... %s\n", errstr(cdr(args)));
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