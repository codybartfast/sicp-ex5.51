#ifndef STRBLDR_H
#define STRBLDR_H
#include "sicpstd.h"
#include "stdbool.h"

struct strbldr {
	char *buff;
	int buffsize;
	int buffidx;
	int (*addc)(struct strbldr *, char);
	int (*adds)(struct strbldr *, char *);
	char *(*string)(struct strbldr *);
	char *(*copy)(struct strbldr *);
	struct strbldr *(*clear)(struct strbldr *);
	bool errored;
	void (*free)(struct strbldr **);
};

struct strbldr *new_strbldr(void);

#endif
