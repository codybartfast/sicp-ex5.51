#ifndef BITMAP_H
#define BITMAP_H
#include "sicpstd.h"

#include "obj.h"

#define CANVAS_WIDTH 1024
#define CANVAS_HEIGHT CANVAS_WIDTH

enum bmpfmt { PGM_P5 };

struct bitmap {
	enum bmpfmt format;
	int width;
	int height;
	int max;
	unsigned char *data;
};

int writebmp(struct bitmap *bmp, const char *path);

extern struct bitmap canvas;
extern const obj hamiltonbmp;
extern const obj patternbmp;
extern const obj rogersbmp;
extern const obj sussmanbmp;

#endif