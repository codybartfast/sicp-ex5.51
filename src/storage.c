#include "storage.h"

#include <stdlib.h>
#include "eval.h"
#include "list.h"

const int blksiz = 4 * (1 << 20); // 4MB
#define MAXBLKS 800 // 800 * 4 MB a bit under 4GB

const int offmax = blksiz / sizeof(struct pair); // pairs per block
struct pair *blocksA[MAXBLKS];
struct pair *blocksB[MAXBLKS];
struct pair **blocks = blocksA;
int blkcnt = 0;
int lstblk = -1;
static int curblk = 0;
static int offset = 0;
static struct pair *next = NULL;
static bool addnext = false;

#include <stdio.h>
#include "error.h"

static struct pair **oldblks;
static obj new;
static struct pair *scan, old;
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
	if (is_broken_heart(old.car)) {
		new = cdr(old.cdr); // ref obj, not the pair?
		return;
	}
	// new location of pair ?
	new = (struct obj){ TYPE_REFERENCE,
			    SUBTYPE_NOT_SET,
			    { .reference = next } };
	// update free pointer
	incnext(); 
	// copy car and cdr to free memory
	set_car(new, old.car);
	set_cdr(new, old.cdr);
	// set car as broken heart
	old.car = broken_heart;
	old.cdr = new;
	//
	// update next, ofset, blk
}

static void relocate_old_result_in_new(void)
{
	if (is_pair(old.car))
		relocate_pair();
	else
		new = old.car;
	return;
}

static struct pair *collect(void)
{
	if (blocks == blocksA) {
		oldblks = blocksA;
		blocks = blocksB;
	} else {
		oldblks = blocksB;
		blocks = blocksA;
	}

	curblk = offset = 0;
	next = blocks[curblk] + offset;
	scnblk = scnoff = 0;
	scan = blocks[scnblk] + scnoff;
	old = *getroot().val.reference;

	relocate_old_result_in_new();
	setroot((struct obj){
		TYPE_REFERENCE, SUBTYPE_NOT_SET, { .reference = next } });

	// gc - loop while (scnoff != offset || scnblk != curblk)
	// {
	// 	old = scan->object;
	// 	relocate_old_result_in_new();
	// }
	// no flip - we started with it instead.
	return next;
}

static struct pair *addblock(void)
{
	struct pair *block;

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

static struct pair *makespace(bool nogc)
{
	if (nogc || addnext) {
		return addblock();
	}
	//next = collect();
	if (curblk == lstblk) {
		if (offset == offmax)
			return addblock();
		if (offset >= (offmax / 2))
			addnext = true;
	}
	return next;
}

static struct pair *getnext(bool nogc)
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

static struct pair *init(void)
{
	return next = makespace(true);
}

struct pair *newpair(bool nogc)
{
	struct pair *rslt;

	if (next == NULL) {
		if (init() == NULL)
			return NULL;
	}
	rslt = next;
	if ((next = getnext(nogc)) == NULL)
		return NULL;
	return rslt;
}
