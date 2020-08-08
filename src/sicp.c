#include <stdbool.h>
#include "version.h"
#include "strbldr.h"
#include "parser.h"
#include "error.h"
#include "eval.h"
#include "output.h"

static struct inport *parseargs(int argc, char *argv[]);
static struct inport *version(void);
static struct inport *expr(int argc, char *argv[]);
static struct inport *usage(void);

int main(int argc, char *argv[])
{
		obj dat;
	struct inport *port = parseargs(argc, argv);
	if (port == NULL)
		return 0;
	while (!iserr(dat = readp(port)) && !Obj.iseof(dat)) {
 		write((dat));
		newline();
	}

	// obj pair1 = cons(Obj.ofint64(11), Obj.ofint64(22));
	// obj pair2 = cons(Obj.ofint64(33), Obj.ofint64(44));
	// obj pair3 = cons(pair1, pair2);
	// obj pair4 = cons(pair1, Obj.ofint64(55));
	// obj p1 = car(pair3);
	// printf("pair1; %ld\n", &pair1);
	// printf("pair1 car: %ld\n", pair1.pair.car.val.int64);
	// printf("pair1 cdr: %ld\n", pair1.pair.cdr.val.int64);
	// printf("pair2; %ld\n", &pair2);
	// printf("pair2 car: %ld\n", pair2.pair.car.val.int64);
	// printf("pair2 cdr: %ld\n", pair2.pair.cdr.val.int64);
	// printf("pair3: %ld\n", &pair3);
	// printf("pair3 car: %ld\n", pair3.pair.car.val.pointer);
	// printf("car(pair3) car: %ld\n", car(pair3).pair.car.val.int64);
	// printf("car(pair3) ispair: %ld\n", car(pair3).ispair);
	// printf("car(pair3) .car.type: %ld\n", car(pair3).pair.car.type);
	// printf("car(car(pair3))  %ld\n", car(car(pair3)).simp.val.int64);
	// printf("cdr(car(pair3))  %ld\n", cdr(car(pair3)).simp.val.int64);
	// printf("car(cdr(pair3))  %ld\n", car(cdr(pair3)).simp.val.int64);
	// printf("cdr(cdr(pair3))  %ld\n", cdr(cdr(pair3)).simp.val.int64);
	// printf("car(car(pair4))  %ld\n", car(car(pair4)).simp.val.int64);
	// printf("cdr(car(pair4))  %ld\n", cdr(car(pair4)).simp.val.int64);
	// printf("cdr(pair4)  %ld\n", cdr(pair4).simp.val.int64);

	return 0;
}

static struct inport *parseargs(int argc, char *argv[])
{
	char *s;
	bool haveoptions;

	if (argc <= 1)
		return openin_ptr(stdin);
	s = argv[1];
	haveoptions = (*s == '-' || *s == '/' || *s == '?');
	if (!haveoptions)
		return (argc == 2) ? openin_file(argv[1]) : usage();

	while (*s == '-' || *s == '/')
		s++;
	switch (*s) {
	case 'v':
	case 'V':
		return version();
	case 'e':
	case 'E':
		return expr(argc, argv);
	case 'h':
	case 'H':
	case '?':
	default:
		return usage();
	}
}

static struct inport *version(void)
{
	printf("sicp version %s\n", SICP_VERSION);
	return NULL;
}

static struct inport *expr(int argc, char *argv[])
{
	struct strbldr *sb;
	int i;

	if (argc <= 2)
		return NULL;
	if ((sb = new_strbldr()) == NULL)
		return NULL;
	for (i = 2; i < argc; i++) {
		if (i > 2)
			sb->addc(sb, ' ');
		sb->adds(sb, argv[i]);
	}
	return openin_string(sb->string(sb));
}

static struct inport *usage(void)
{
	printf("usage: [<path> | --expr <expression> | --version| --help]\n"
	       "  <path>                    evaluate contents of file <path>\n"
	       "  -e, --expr <expression>   evaluate expression <expression>\n"
	       "  -v, --version             display version information\n"
	       "  -h, --help                display this usage information\n"
	       "\n"
	       "If there are no arguments, will read and evaluate standard in.\n");
	return NULL;
}
