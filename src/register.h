#ifndef REGISTER_H
#define REGISTER_H
#include "sicpstd.h"

#include "obj.h"

struct core {
	obj argl;
	obj cont;
	obj env;
	obj expr;
	obj proc;
	obj stack;
	obj unev;
	obj val;
};

struct core *dfltcore(void);

extern obj anenv;
extern obj ambenv;
extern obj savetmp;

obj getroot(void);
obj setroot(obj);
#endif