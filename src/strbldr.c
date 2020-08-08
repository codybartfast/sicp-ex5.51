
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "strbldr.h"
#include "windows.h"

#define AREA "strbldr"
#define INITIAL_BUFFSIZE (1 << 10)

static int addc(struct strbldr *, char);
static int adds(struct strbldr *, char *);
static char *str(struct strbldr *);
static char *copy(struct strbldr *);
static struct strbldr *clear(struct strbldr *);
static void sbfree(struct strbldr *);
static int grow_buff(struct strbldr *);

struct strbldr *new_strbldr(void)
{
	struct strbldr *sb = malloc(sizeof(struct strbldr));
	if (sb == NULL) {
		eprintf(AREA, "No memory for strbldr");
		return NULL;
	}
	sb->buff = calloc(sizeof(char), INITIAL_BUFFSIZE);
	if (sb->buff == NULL) {
		eprintf(AREA, "No memory for strbldr buff");
		return NULL;
	}
	sb->buffsize = INITIAL_BUFFSIZE;
	sb->buffidx = 0;
	sb->addc = addc;
	sb->adds = adds;
	sb->string = str;
	sb->copy = copy;
	sb->clear = clear;
	sb->free = sbfree;
	return sb;
}

static int addc(struct strbldr *sb, char c)
{
	if (sb->buffidx >= sb->buffsize - 1) {
		if (grow_buff(sb) != 0)
			return EOF;
	}
	return *(sb->buff + sb->buffidx++) = c;
}

static int adds(struct strbldr *sb, char *s)
{
	int rc = 0;

	if (s == NULL)
		return EOF;
	while (*s != '\0' && rc != EOF)
		rc = addc(sb, *s++);
	return (rc == EOF) ? EOF : 0;
}

static char *str(struct strbldr *sb)
{
	*(sb->buff + sb->buffidx) = '\0';
	return sb->buff;
}

static char *copy(struct strbldr *sb)
{
	long dlen;
	*(sb->buff + sb->buffidx) = '\0';

	char *str = (char *)malloc((dlen = (sb->buffidx + 1)) * sizeof(char));
	if (str == NULL) {
		eprintf(AREA, "No memory to copy strbldr string");
		return NULL;
	}
	strcpy_s(str, dlen, sb->buff);
	printf("copy '%s'->'%s' (%d)(%ld)\n", sb->buff, str, sb->buffidx, dlen);
	return str;
}

static struct strbldr *clear(struct strbldr *sb)
{
	if (sb != NULL)
		sb->buffidx = 0;
	return sb;
}

static int grow_buff(struct strbldr *sb)
{
	int i;
	char *old_buff = sb->buff;

	sb->buffsize *= 2;
	sb->buff = (char *)malloc(sb->buffsize * sizeof(char));
	if (sb->buff == NULL) {
		free(old_buff);
		return -1;
	}
	for (i = 0; i < sb->buffidx; i++)
		*(sb->buff + i) = *(old_buff + i);
	free(old_buff);
	return 0;
}

static void sbfree(struct strbldr *sb)
{
	if (sb != NULL) {
		free(sb->buff);
		sb->buff = NULL;
		free(sb);
	}
}
