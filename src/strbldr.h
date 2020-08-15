#ifndef STRBLDR_H
#define STRBLDR_H
#include "sicpstd.h"
#include "stdbool.h"

struct strbldr {
	char *buff;
	int buffsize;
	int buffidx;
	bool errored;
};

struct strbldr *new_strbldr(void);

int sb_addc(struct strbldr *, char);
int sb_adds(struct strbldr *, const char *);
char *sb_string(struct strbldr *);
char *sb_copy(struct strbldr *);
struct strbldr *sb_clear(struct strbldr *);
void sb_free(struct strbldr **);

#endif
