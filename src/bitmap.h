#ifndef BITMAP_H
#define BITMAP_H
#include "sicpstd.h"

enum bmfmt { PGM_P5 };

struct bitmap {
	enum bmfmt format;
	int width;
	int height;
	int max;
	unsigned char data[42436];
};

unsigned char *bmp2arr(const struct bitmap *restrict bmp);

extern const struct bitmap rogers;
extern const struct bitmap sussman;

extern const unsigned char wbrogers[];
extern const int wbrlen;
extern const unsigned char gjsussman[];
extern const int gjslen;

#endif