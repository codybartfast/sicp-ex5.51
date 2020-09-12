#ifndef CHARACTER_H
#define CHARACTER_H
#include "sicpstd.h"

enum bmfmt {
	PGM_P5
};

struct bitmap {
	enum bmfmt format;
	int width;
	int height;
	int max;
	int data[];
};

extern struct bitmap rogers;
extern struct bitmap sussman;

#endif