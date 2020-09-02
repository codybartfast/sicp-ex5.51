#include "storage.h"

#include <stdlib.h>
#include "eval.h"
#include "list.h"

#define AREA "STORAGE"

const int blksiz = 4 * (1 << 20); // 4MB
#define MAXBLKS 800 // 800 * 4 MB a bit under 4GB

static int offmax; // pairs per block
static struct pair *blocksA[MAXBLKS];
static struct pair *blocksB[MAXBLKS];
static struct pair **blocks = blocksA;
static int blkcnt = 0;
static int lstblk = -1;
static int curblk = 0;
static int offset = 0;
static struct pair *next = NULL;
static bool addnext = false;

#include <stdio.h>
#include "error.h"

static struct pair **oldblks;
static obj old, new;
static struct pair *scan;
static int scnblk = 0, scnoff = 0;

// static void display(obj lst, int depth, char *msg)
// {
// 	return;
// 	// obj fst;
// 	// if (msg != NULL) {
// 	// 	printf("\n**********\n%s\n**********\n", msg);
// 	// }
// 	// if (!is_pair(lst)) {
// 	// 	if (is_null(lst))
// 	// 		printf("  END OF LIST\n");
// 	// 	else {
// 	// 		printf("  IMPROPER - ends with %s\n", errstr(lst));
// 	// 	}
// 	// 	return;
// 	// }
// 	// fst = car(lst);
// 	// printf("  Pair: [%p]\n", lst.val.reference);
// 	// if (is_pair(fst)) {
// 	// 	printf("    list: (%s ...)\n", errstr(car(fst)));
// 	// } else {
// 	// 	printf("    car: %s\n", errstr(fst));
// 	// }
// 	// display(cdr(lst), depth - 1, NULL);
// 	// if (msg != NULL)
// 	// 	printf("\n");
// }

static void incnext(void)
{
	if (++offset == offmax) {
		curblk++;
		offset = 0;
	}
	next = blocks[curblk] + offset;

	//printf("snack? %d %d\n", curblk, offset);
}

static void incscan(void)
{
	if (++scnoff == offmax) {
		scnblk++;
		scnoff = 0;
	}
	scan = blocks[scnblk] + scnoff;
}

static void relocate_pair(void)
{
	if (is_broken_heart(car(old))) {
		// "Dereferencing broken heart\n");

		new = cdr(old); // ref obj, not the pair?
		return;
	}
	// new location for pair
	new = of_pair(next);
	// update free pointer
	incnext();
	// copy car and cdr to free memory
	set_car(new, car(old));
	set_cdr(new, cdr(old));
	// set car as broken heart
	set_car(old, broken_heart);
	set_cdr(old, new);
	// printf("Relocating: %p -> %p\n", old.val.reference, new.val.reference);
}

static void relocate_old_result_in_new(void)
{
	if (is_pair(old)) {
		relocate_pair();
	} else {
		new = old;
	}
	return;
}

static struct pair *collect(void)
{
	obj newroot;
	// printf("COLLECTING\n");

	if (blocks == blocksA) {
		oldblks = blocksA;
		blocks = blocksB;
	} else {
		oldblks = blocksB;
		blocks = blocksA;
	}

	curblk = offset = 0;
	scnblk = scnoff = 0;
	scan = next = *blocks;
	old = getroot();

	newroot = old;
	// display(newroot, 5, "START");

	relocate_old_result_in_new();
	newroot = new;
	// display(newroot, 5, "MOVE ROOT");

	// gc-loop
	while (scnoff != offset || scnblk != curblk) {
		old = scan->car;
		relocate_old_result_in_new();
		scan->car = new;
		old = scan->cdr;
		relocate_old_result_in_new();
		scan->cdr = new;
		incscan();
	}

	// no flip - we started with it instead.
	// display(newroot, 5, "NIGH");

	// int i;
	// printf("Original:   %p\n", *blocksA);
	// printf("Compressed: %p\n", *blocksB);
	// for (int i = 0; i <= offset; i++) {
	// 	struct pair *addr = blocks[curblk] + i;
	// 	printf("Addr: %p ", addr);
	// 	if (is_pair(addr->car)) {
	// 		printf("car:%p ", addr->car.val.reference);
	// 	} else {
	// 		printf("car:------%d------- ", addr->car.type);
	// 	}
	// 	if (is_pair(addr->cdr)) {
	// 		printf("cdr:%p ", addr->cdr.val.reference);
	// 	} else {
	// 		printf("car:------%d------- ", addr->cdr.type);
	// 	}
	// 	printf("\n");
	// }

	setroot(newroot);
	return next;
}

static struct pair *addblock(void)
{
	struct pair *block;
	// printf("ADDBLOCK\n");

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
	curblk = lstblk = blkcnt++;
	offset = 0;
	next = blocks[lstblk];
	// if (next == NULL)
	// 	printf("Where's my lunch?\n");
	return next;
}

static struct pair *makespace(bool gc)
{
	long all, usd;
	if (!gc || addnext) {
		return addblock();
	}
	next = collect();
	// printf("POST: used %d / %d\n", offset, offmax);

	if (curblk == lstblk && offset == offmax) {
		return addblock();
	}

	all = blkcnt * offmax;
	usd = (curblk * offmax) + offset;
	// printf("RAM: %ld, All: %ld, usd: %ld\n", all * sizeof(struct pair), all, usd);
	if(2 * usd > all)
		addnext = true;
	return next;}

static struct pair *getfree(bool gc)
{
	int avail;
	offset++;
	if (curblk < lstblk) {
		if (offset < offmax) {
			return next = blocks[curblk] + offset;
		} else {
			return next = blocks[++curblk] + (offset = 0);
		}
	}

	avail = offmax - offset;
	if (avail == 0) {
		return makespace(gc);
	} else if (gc && avail < offmax / 16) {
		return makespace(gc);
	} else {
		return next = blocks[curblk] + offset;
	}
}

static struct pair *init(void)
{
	offmax = blksiz / sizeof(struct pair);
	return next = makespace(false);
}

struct pair *newpair(bool gc)
{
	struct pair *rslt;

	if (next == NULL) {
		// printf("Early in the Morning\n");
		if (init() == NULL)
			return NULL;
	}
	// if (next == NULL)
	// 	printf("coffee\n");
	struct pair *gnr = getfree(gc);
	rslt = gnr;
	// printf("NEWPAIR: gnr - %p\n", gnr);
	if ((gnr) == NULL) {
		// printf("Late at night\n");
		return NULL;
	}
	if (next == NULL) {
		// printf("In the midnight hour\n");
	}
	// printf("NEWPAIR: final - %p\n", rslt);
	return rslt;
}
