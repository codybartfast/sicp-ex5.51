#include "pict.h"

#include <stdlib.h>

struct bitmap *canvas = NULL;

#include <stdio.h>
obj paintp(obj args){
	(void)args;
	printf("Painting ...\n");
	return _void;
}

obj write_canvas(obj args){
	(void)args;
	printf("Displaying canvas ...\n");
	return _void;
}

obj write_if_canvas(obj args){
	if(canvas != NULL){
		write_canvas(args);
	}
	return _void;
}