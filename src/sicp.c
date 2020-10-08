#include "sicpstd.h"

#include <string.h>
#include "custom.h"
#include "environment.h"
#include "eval.h"
#include "load.h"
#include "output.h"
#include "parser.h"
#include "primproc.h"
#include "pict.h"
#include "version.h"

static struct inport *parseargs(int argc, char *argv[]);
static struct inport *version(void);
static struct inport *expr(int argc, char *argv[]);
static struct inport *usage(void);

#include "bitmap.h"

int main(int argc, char *argv[])
{
	obj exp;
	struct inport *in = parseargs(argc, argv);
	if (in == NULL)
		return 0;
	struct outport *out = openout_ptr(stdout);
	if (out == NULL)
		return 0;

	exp = do_head(tge(), in);

	load_u(in, openout_ptr(stdout), &exp, true);
	newlinep(out);
	write_canvas_if_painted(emptylst);

	parser_freetemp();
	out_close(out);
	in_close(in);
	return 0;
}

static struct inport *parseargs(int argc, char *argv[])
{
	char *s;
	bool haveoptions;

	if (argc <= 1)
		return openin_ptr(stdin);
	s = argv[1];
	haveoptions = (*s == '-' || *s == '?' || (*s == '/' && strlen(s) == 2));
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
	char *s;
	int i;

	if (argc <= 2)
		return NULL;
	if ((sb = new_strbldr()) == NULL)
		return NULL;
	for (i = 2; i < argc; i++) {
		if (i > 2)
			sb_addc(sb, ' ');
		sb_adds(sb, argv[i]);
	}
	s = sb_copy(sb);
	sb_free(&sb);
	if (s == NULL)
		return NULL;
	return openin_string(s);
}

static struct inport *usage(void)
{
	printf("usage: [<path> | --expr <expression> | --version | --help]\n"
	       "  <path>                    evaluate contents of file <path>\n"
	       "  -e, --expr <expression>   evaluate expression <expression>\n"
	       "  -v, --version             display version information\n"
	       "  -h, --help                display this usage information\n"
	       "\n"
	       "If there are no arguments, will read and evaluate standard in.\n"
	       "\n"
	       "Contains code and images licensed under Creative Commons (CC \n"
	       "BY-SA 3.0 & 4.0) by MIT, Hal Abelson and Gerald Jay Sussman.\n");
	return NULL;
}
