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

#include "bitmap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char *argv[])
{
	// int c;
	// int i = 14;

	// while ((c = getchar()) != EOF) {
	// 	printf("%d", c);
	// 	if (i-- == 0) {
	// 		i = 179;
	// 		printf(" ");
	// 		// printf("\n");
	// 	} else {
	// 		printf(" ");
	// 	}
	// }
	// return 0;

	printf("%d\n", gjslen);

	stbi_uc *buff = (stbi_uc *)gjsussman;
	int width;
	int height;
	int channels;

	// unsigned char *img = stbi_load("images/gjsussman-p5.pgm", &width,
	// 			       &height, &channels, 0);
	unsigned char *img = stbi_load_from_memory(gjsussman, gjslen, &width,
						   &height, &channels, 0);
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n",
	       width, height, channels);
	stbi_write_png("pict.png", width, height, channels, img,
		       width * channels);
	return 0;

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
	       "If there are no arguments, will read and evaluate standard in.\n"
	       "\n"
	       "Contains code and images licensed under Creative Commons (CC \n"
	       "BY-SA 3.0 & 4.0) by MIT, Hal Abelson and Gerald Jay Sussman.\n");
	return NULL;
}
