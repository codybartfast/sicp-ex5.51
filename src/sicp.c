#include "sicpstd.h"

#include <string.h>
#include "custom.h"
#include "environment.h"
#include "eval.h"
#include "output.h"
#include "parser.h"
#include "version.h"

static struct inport *parseargs(int argc, char *argv[]);
static struct inport *version(void);
static struct inport *expr(int argc, char *argv[]);
static struct inport *usage(void);

int main(int argc, char *argv[])
{
	obj exp;
	struct inport *port = parseargs(argc, argv);
	if (port == NULL)
		return 0;

	exp = do_head(tge(), port);
	for (; !is_err(exp) && !is_eof(exp); exp = readp(port)) {
		obj dat = eval(exp, tge());
		if (is_err(dat))
			break;
		if (is_void(dat))
			continue;
		write(dat);
		newline(emptylst);
	}
	newline(emptylst);

	parser_freetemp();
	in_close(port);
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
	       "If there are no arguments, will read and evaluate standard in.\n");
	return NULL;
}
