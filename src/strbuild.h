#ifndef STRBUILD_H
#define STRBUILD_H 1

struct strbuild {
	char *buff;
	int buffsize;
	int buffidx; 
	int (*addc)(struct strbuild *, char);
	char *(*string)(struct strbuild *);
	char *(*copy)(struct strbuild *);
	struct strbuild *(*clear)(struct strbuild *);
	void (*free)(struct strbuild *);
};

struct strbuild *new_strbuild(void);

#endif
