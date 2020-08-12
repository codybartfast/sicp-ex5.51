#ifndef PARSER_H
#define PARSER_H
#include "sicpstd.h"

#include "inport.h"
#include "obj.h"

obj read(void);
obj readp(struct inport *);
void parser_freetemp(void);

#endif
