#ifndef BITMAP_H
#define BITMAP_H
#include "sicpstd.h"

enum bmpfmt { PGM_P5 };

struct bitmap {
	enum bmpfmt format;
	int width;
	int height;
	int max;
	const unsigned char *data;
};

int writebmp(const struct bitmap *restrict bmp);

extern const struct bitmap rogersbmp;
extern const struct bitmap sussmanbmp;

#endif