#include "parser.h"
#include "eval.h"
#include "convert.h"

#include "out_port.h"

int main(void)
{
	obj *num;
	struct in_port *in =
		open_input_string(" 1 23 345 4567 123456789012345678 ");
	num = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);
	num = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);
	num = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);
	num = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);
	num = read(in);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);
	       
	num = eval(num);
	printf("type: %d, subtype: %d, value: %ld\n", num->type, num->subtype,
	       num->val.int64);

	obj *str = cnv_number_string(num);
	printf("type: %d, subtype: %d, value: \"%s\"\n", str->type,
	       str->subtype, str->val.string);

	struct out_port *out = open_output_string();
	out->writes(out, "rage against the meh-chine\nblah\n");
	out->writes(out, "phew\n");
	out->writes(out, "phew\n");
	printf("%s", out->string(out));

	return 0;
}
