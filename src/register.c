#include "register.h"

#include <stdlib.h>
#include "environment.h"
#include "error.h"
#include "list.h"

#define AREA "REGISTER"

static struct core cores[NCORE];

// global-env
obj ambenv;
obj anenv;
obj svtmp;

const int rootlen = NOTHER + NCORE;
static obj rootlst;

static void init(void)
{
	static bool initdone = false;
	if (initdone)
		return;

	rootlst = emptylst;
	int i, j;
	for (i = 0; i < NCORE; i++) {
		obj reglst = emptylst;
		for (j = 0; j < NREG; j++) {
			reglst = cons(unspecified, reglst);
		}
		rootlst = cons(reglst, rootlst);
	}
	for (i = 0; i < NOTHER; i++) {
		rootlst = cons(unspecified, rootlst);
	}
	initdone = true;
}

obj getroot(void)
{
	int i;
	obj lst = rootlst;

	set_car(lst, the_global_environment());
	lst = cdr(lst);

	set_car(lst, ambenv);
	lst = cdr(lst);

	set_car(lst, anenv);
	lst = cdr(lst);

	set_car(lst, svtmp);
	lst = cdr(lst);

	for (i = 0; i < NCORE; i++, lst = cdr(lst)) {
		struct core *cr = &cores[i];
		obj reglst = car(lst);

		set_car(reglst, cr->argl);
		reglst = cdr(reglst);

		set_car(reglst, cr->cont);
		reglst = cdr(reglst);

		set_car(reglst, cr->env);
		reglst = cdr(reglst);

		set_car(reglst, cr->expr);
		reglst = cdr(reglst);

		set_car(reglst, cr->proc);
		reglst = cdr(reglst);

		set_car(reglst, cr->stack);
		reglst = cdr(reglst);

		set_car(reglst, cr->unev);
		reglst = cdr(reglst);

		set_car(reglst, cr->val);
		reglst = cdr(reglst);
	}

	return rootlst;
}

void setroot(obj rlst)
{
	int i;
	obj lst = rootlst = rlst;

	set_global_environment(car(lst));
	lst = cdr(lst);

	ambenv = car(lst);
	lst = cdr(lst);

	anenv = car(lst);
	lst = cdr(lst);

	svtmp = car(lst);
	lst = cdr(lst);

	for (i = 0; i < NCORE; i++, lst = cdr(lst)) {
		struct core *cr = &cores[i];
		obj reglst = car(lst);

		cr->argl = car(reglst);
		reglst = cdr(reglst);

		cr->cont = car(reglst);
		reglst = cdr(reglst);

		cr->env = car(reglst);
		reglst = cdr(reglst);

		cr->expr = car(reglst);
		reglst = cdr(reglst);

		cr->proc = car(reglst);
		reglst = cdr(reglst);

		cr->stack = car(reglst);
		reglst = cdr(reglst);

		cr->unev = car(reglst);
		reglst = cdr(reglst);

		cr->val = car(reglst);
		reglst = cdr(reglst);
	}
}

struct core *getcore(int idx)
{
	if (idx < 0 || NCORE <= idx) {
		eprintf(AREA, "Invalid core index: %d", idx);
		exit(1);
	}
	init();
	return &cores[idx];
}

struct core *dfltcore(void)
{
	init();
	return &cores[0];
}
