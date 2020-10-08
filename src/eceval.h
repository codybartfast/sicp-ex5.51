#ifndef ECEVAL_H
#define ECEVAL_H
#include "sicpstd.h"

#include "obj.h"

extern obj anenv;
obj eceval(obj exp, obj env);
obj getroot(void);
obj setroot(obj);

#endif
