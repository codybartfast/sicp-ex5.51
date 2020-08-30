#include "storage.h"

#include <stdlib.h>

// cell (at time of writinng) is 40 or 80 bytes, so using a multiple of five.
const int blksiz = 5 * (1 << 20); // 5MB
#define MAXBLKS 300 // 800 * 5 MB a bit under 4GB

const int offmax = blksiz / sizeof(struct cell); // cells per block
struct cell *blocksA[MAXBLKS];
struct cell *blocksB[MAXBLKS];
struct cell **blocks;
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

struct cell *newcell(bool nogc)
{
	struct cell *rslt;

	if (next == NULL) {
		// init
		blocks = blocksB;
		next = makespace(nogc);
		if (next == NULL)
			return NULL;
	}
	rslt = next;
	next = getnext(nogc);
	if (next == NULL)
		return NULL;
	return rslt;
}
