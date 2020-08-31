#include "storage.h"

#include <stdlib.h>
#include "eval.h"
#include "list.h"

const int blksiz = 4 * (1 << 20); // 4MB
#define MAXBLKS 800 // 800 * 4 MB a bit under 4GB

const int offmax = blksiz / sizeof(struct cell); // cells per block
struct cell *blocksA[MAXBLKS];
struct cell *blocksB[MAXBLKS];
struct cell **blocks = blocksA;
int blkcnt = 0;
int lstblk = -1;
static int curblk = 0;
static int offset = 0;
static struct cell *next = NULL;
static bool addnext = false;

#include <stdio.h>
#include "error.h"

static struct cell **oldblks;
static obj old;
static struct cell *scan;
static int scnblk, scnoff;

static void incnext(void)
{
	if (offset == offmax) {
		curblk++;
		offset = 0;
	}
	next = blocks[curblk] + offset++;
}

static void relocate_pair(void)
{
	obj oldcar = car(old);
	if (is_broken_heart(oldcar)) {
		next->object = cdr(old); // ref obj, not the pair?
		return;
	}
	// use next to store pair ?
	*next = (struct cell){ .pair = { old, cdr(old) } };
	// set car as broken heart
	set_car(old, broken_heart);
	// set cdr as pointer to new location
	set_cdr(old, (struct obj){ TYPE_REFERENCE,
				   SUBTYPE_NOT_SET,
				   { .reference = next } });

	// advance nxt
	incnext();
	//
	// update next, ofset, blk
}

static void relocate_old_result_in_new(void)
{
	if (is_pair(old))
		relocate_pair();
	else
		next->object = old;
	return;
}

static struct cell *collect(void)
{
	// if (blocks == blocksA) {
	// 	oldblks = blocksA;
	// 	blocks = blocksB;
	// } else {
	// 	oldblks = blocksB;
	// 	blocks = blocksA;
	// }

	// curblk = offset = 0;
	// next = blocks[curblk] + offset;
	// scnblk = scnoff = 0;
	// scan = blocks[scnblk] + scnoff;
	// old = getroot();

	// relocate_old_result_in_new();
	// setroot((struct obj){
	// 	TYPE_REFERENCE, SUBTYPE_NOT_SET, { .reference = next } });

	// gc-loop
	// while (scnoff != offset || scnblk != curblk) {
	// 	old = scan->object;
	// 	relocate_old_result_in_new();

	// }
	// no flip - we started with it instead.
	return next;
}

static struct cell *addblock(void)
{
	struct cell *block;

	addnext = false;
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
	return blocks[lstblk = blkcnt++];
}

static struct cell *makespace(bool nogc)
{
	if (nogc || addnext) {
		return addblock();
	}
	next = collect();
	if (curblk == lstblk) {
		if (offset == offmax)
			return addblock();
		if (offset >= (offmax / 2))
			addnext = true;
	}
	return next;
}

static struct cell *getnext(bool nogc)
{
	if (offset == offmax) {
		if (curblk == lstblk) {
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
	struct cell *rslt;

	if (next == NULL) {
		if (init() == NULL)
			return NULL;
	}
	rslt = next;
	if ((next = getnext(nogc)) == NULL)
		return NULL;
	return rslt;
}
