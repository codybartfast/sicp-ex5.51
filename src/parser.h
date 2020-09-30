#ifndef PARSER_H
#define PARSER_H
#include "sicpstd.h"

#include "inport.h"
#include "obj.h"

obj read(void);
obj read_p(obj args);
obj readp(struct inport *);
void parser_freetemp(void);

#endif
