#include "register.h"

#include <stdlib.h>
#include "environment.h"
#include "error.h"
#include "list.h"

#define AREA "REGISTER"

struct core core0;
struct core *cr0 = &core0;

// tge       //  9
obj anenv; // 10
obj ambenv; // 11
obj savetmp; // 12

const int rootlen = 12;
static obj rootlst;

obj getroot(void)
{
	int actlen;
	obj lst = rootlst;

	// intentionally not using rootlen here, change number manually after
	// modifying body below.
	if ((actlen = length_u(rootlst)) != 12) {
		error_internal(
			AREA,
			"Bug! getroot() got list of unexpected length: %d ",
			actlen);
		exit(1);
	}
	// order must match setroot
	set_car(lst, cr0->argl);
	lst = cdr(lst);
	set_car(lst, cr0->cont);
	lst = cdr(lst);
	set_car(lst, cr0->env);
	lst = cdr(lst);
	set_car(lst, cr0->expr);
	lst = cdr(lst);
	set_car(lst, cr0->proc);
	lst = cdr(lst);
	set_car(lst, cr0->stack);
	lst = cdr(lst);
	set_car(lst, cr0->unev);
	lst = cdr(lst);
	set_car(lst, cr0->val);
	lst = cdr(lst);
	set_car(lst, the_global_environment());
	lst = cdr(lst);
	set_car(lst, anenv);
	lst = cdr(lst);
	set_car(lst, ambenv);
	lst = cdr(lst);
	set_car(lst, savetmp);

	return rootlst;
}

// used for garbage collection
obj setroot(obj rlst)
{
	int actlen;
	obj lst = rootlst = rlst;

	// intentionally not using rootlen here, change number manually after
	// modifying body below.
	if ((actlen = length_u(rootlst)) != 12) {
		return error_internal(
			AREA,
			"Bug! setroot() got list of unexpected length: %d",
			actlen);
	}
	// order must match getroot
	cr0->argl = car(lst);
	lst = cdr(lst);
	cr0->cont = car(lst);
	lst = cdr(lst);
	cr0->env = car(lst);
	lst = cdr(lst);
	cr0->expr = car(lst);
	lst = cdr(lst);
	cr0->proc = car(lst);
	lst = cdr(lst);
	cr0->stack = car(lst);
	lst = cdr(lst);
	cr0->unev = car(lst);
	lst = cdr(lst);
	cr0->val = car(lst);
	lst = cdr(lst);
	set_global_environment(car(lst));
	lst = cdr(lst);
	anenv = car(lst);
	lst = cdr(lst);
	ambenv = car(lst);
	lst = cdr(lst);
	savetmp = car(lst);

	return unspecified;
}

static bool initdone = false;
static obj init(void)
{
	int actlen;

	cr0->stack = emptylst;
	// preallocate storage for gc root
	rootlst = listn(12, //       <----- actual  length
			unspecified, //  1
			unspecified, //  2
			unspecified, //  3
			unspecified, //  4
			unspecified, //  5
			unspecified, //  6
			unspecified, //  7
			unspecified, //  8
			unspecified, //  9
			unspecified, // 10
			unspecified, // 11
			unspecified //  12
	);
	if ((actlen = length_u(rootlst)) != rootlen) {
		error_internal(
			AREA,
			"Bug! init, root wrong length. is: %d, expected %d",
			actlen, rootlen);
		exit(1);
	}
	initdone = true;
	return unspecified;
}

struct core *dfltcore(void)
{
	if (!initdone) {
		init();
	}
	return cr0;
}
