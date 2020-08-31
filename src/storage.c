#include "storage.h"

#include <stdlib.h>

// cell (at time of writinng) is 40 or 80 bytes, so using a multiple of five.
const int blksiz = 5 * (1 << 20); // 5MB
#define MAXBLKS 800 // 800 * 5 MB a bit under 4GB

const int offmax = blksiz / sizeof(struct cell); // cells per block
struct cell *blocksA[MAXBLKS];
struct cell *blocksB[MAXBLKS];
struct cell **blocks = blocksA;
int blkcnt = 0;
static int curblk = 0;
static int offset = 0;
static struct cell *next = NULL;

#include <stdio.h>

static struct cell *addblock(void)
{
	struct cell *block;

	if (blkcnt >= MAXBLKS)
		return NULL;
	block = calloc(blksiz, 1);
	if (block == NULL)
		return NULL;
	blocksA[blkcnt] = block;
	block = calloc(blksiz, 1);
	if (block == NULL)
		return NULL;
	blocksB[blkcnt] = block;
	return blocks[blkcnt++];
}

static struct cell *makespace(bool nogc)
{
	printf("Making Space: %d\n", blkcnt);
	(void)nogc;
	return addblock();
}

static struct cell *getnext(bool nogc)
{
	if (offset == offmax) {
		if (curblk + 1 == blkcnt) {
			if (makespace(nogc) == NULL)
				return NULL;
		}
		curblk++;
		offset = 0;
	}
	return blocks[curblk] + offset++;
}

static struct cell *init(void)
{
	return next = makespace(true);
}

struct cell *newcell(bool nogc)
{
	struct cell *new;

	if (next == NULL) {
		if (init() == NULL)
			return NULL;
	}
	new = next;
	if ((next = getnext(nogc)) == NULL)
		return NULL;
	return new;
}
