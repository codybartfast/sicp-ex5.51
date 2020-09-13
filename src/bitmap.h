#ifndef BITMAP_H
#define BITMAP_H
#include "sicpstd.h"

#include "obj.h"

enum bmpfmt { PGM_P5 };

struct bitmap {
	enum bmpfmt format;
	int width;
	int height;
	int max;
	const unsigned char *data;
};

int writebmp(const struct bitmap *restrict bmp);

extern const obj rogersbmp;
extern const obj sussmanbmp;

#endif