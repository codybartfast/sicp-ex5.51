#include "parser.h"

int main(void)
{
	obj *obj;
	struct in_port *in = open_input_string(" 1 2  123456789012345678 ");
	obj = read(in);
	obj = read(in);
	obj = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", obj->type, obj->subtype,
	       obj->value.int64);
	return 0;
}
