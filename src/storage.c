#include "storage.h"

#include <stdlib.h>
#include "eval.h"
#include "list.h"

#define AREA "STORAGE"

// maximum usable ram = blksiz * maxblks
// maximum allocation = blksiz * (maxblks * 2)
// maximum usable pairs = blksiz * maxblks / sizeof(pair)
// e.g. If blksiz = (1 << 20), MAXBLKS = 1024 & sizeof(pair) = 32
//      initial = 1MB usable, 2MB allocated, 32,768 pairs available.
// 	max usable = 1GB
//	max allocated = 2GB
//      max available pairs = (32,768 * 1,024) = 2,834,432
const int blksiz = (1 << 20);
#define MAXBLKS 1024

bool disable_gc = false;

static int offmax; // pairs per block
static int low_avail; // threshold for garbage collection
static struct pair *blocksA[MAXBLKS];
static struct pair *blocksB[MAXBLKS];
static struct pair **blocks = blocksA;
static int blkcnt;
static int lstblk;
static bool addnext = false;

static struct pair *next;
static int curblk, offset;
static struct pair *nextfree(void)
{
	struct pair *rslt = next;

	if (++offset == offmax) {
		curblk++;
		offset = 0;
	}
	next = blocks[curblk] + offset;
	return rslt;
}

static struct pair *scan;
static int scnblk, scnoff;
static struct pair *nextscan(void)
{
	struct pair *rslt = scan;

	if (++scnoff == offmax) {
		scnblk++;
		scnoff = 0;
	}
	scan = blocks[scnblk] + scnoff;
	return rslt;
}

static void update_obj(obj *objptr)
{
	// Receives an address in the new blocks.  But if that address contains
	// a pointer to a pair (i.e., is_pair) then we know that pair pointer
	// always points to a location in the old block.
	obj old;
	obj new;

	if (!is_pair(*objptr)) {
		// not a pair pointer - nothing to do.
		return;
	}
	// address in the old blocks that pair pointer still points to.
	old = *objptr;
	if (is_broken_heart(car(old))) {
		// Pair pointer points to an old address that has already
		// been copied, update the scan's car or cdr to point to the
		// address the pair data was copied to.
		*objptr = cdr(old);
		return;
	}
	// The pair pointer points to an address in the old blocks and we
	// have not yet copied the data to the new blocks.

	// Get a free address in the new blocks for the pair data.
	new = of_pair(nextfree());
	// Copy car from pair in old blocks to the car of pair in new blocks
	set_car(new, car(old));
	// And the cdr
	set_cdr(new, cdr(old));

	// set car of pair at the old address as a broken heart
	set_car(old, broken_heart);
	// set cdr of pair at the old address as the address of the new pair
	set_cdr(old, new);

	// update the scan's car or cdr to be the new address of the new pair
	*objptr = new;
}

static struct pair *collect_garbage(void)
{
	// pull reg values from evaluator
	obj root = getroot();

	// gc-flip (at start instead of end)
	blocks = (blocks == blocksA) ? blocksB : blocksA;

	// begin garbage collection
	curblk = offset = 0;
	scnblk = scnoff = 0;
	scan = next = *blocks;

	// relocate root
	update_obj(&root);

	// gc-loop
	while (scnoff != offset || scnblk != curblk) {
		update_obj(&scan->car);
		update_obj(&scan->cdr);
		nextscan();
	}

	// push new reg values back into the evaluator
	setroot(root);
	return next;
}

static struct pair *addblock(void)
{
	struct pair *block;

	addnext = false;
	if (blkcnt == MAXBLKS) {
		return NULL;
	}
	block = calloc(blksiz, 1);
	if (block == NULL) {
		return NULL;
	}
	blocksA[blkcnt] = block;
	block = calloc(blksiz, 1);
	if (block == NULL) {
		return NULL;
	}
	blocksB[blkcnt] = block;
	curblk = lstblk = blkcnt++;
	return next = blocks[curblk] + (offset = 0);
}

static struct pair *makespace(bool gc_safe)
{
	if (!gc_safe || addnext || disable_gc) {
		return addblock();
	}
	next = collect_garbage();
	if (curblk == lstblk && offset == (offmax - 1)) {
		return addblock();
	} else {
		long capacity = blkcnt * offmax;
		long used = (curblk * offmax) + offset;
		if (used > capacity / 4) {
			addnext = true;
		}
		return next;
	}
}

static struct pair *init(void)
{
	offmax = blksiz / sizeof(struct pair);
	low_avail = offmax / 128;
	low_avail = (low_avail < 1) ? 1 : (low_avail > 256) ? 256 : low_avail;
	addnext = true;
	return makespace(false);
}

static struct pair *getfree(bool gc_safe)
{
	if (curblk == lstblk) {
		int avail = (offmax - offset);
		if (avail <= low_avail && (avail == 1 || gc_safe)) {
			if (makespace(gc_safe) == NULL) {
				return NULL;
			}
		}
	}
	return nextfree();
}

bool needinit = true;
struct pair *newpair(bool gc_safe)
{
	if (needinit) {
		needinit = false;
		if (init() == NULL) {
			return NULL;
		}
	}
	return getfree(gc_safe);
}
