#include <stdio.h>
#include "value.h"

int main(void)
{
	value v = { 0, 0, 0, 0, 0 };
	int32_value(-51, &v);
	printf("%lu\n", *((unsigned long *)(void *)&v));
	printf("%d\n", value_int32(&v));
	return 0;
}

/* Output

4294967245
-51

*/
