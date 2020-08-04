
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "strbldr.h"
#include "windows.h"

#define AREA "strbldr"
#define INITIAL_BUFFSIZE (1 << 2)

static int addc(struct strbldr *sb, char c);
static char *str(struct strbldr *sb);
static char *copy(struct strbldr *sb);
static struct strbldr *clear(struct strbldr *sb);
static void sbfree(struct strbldr *sb);
static int grow_buff(struct strbldr *sb);

struct strbldr *new_strbldr(void)
{
	struct strbldr *sb = malloc(sizeof(struct strbldr));
	if (sb == NULL) {
		error(AREA, "No memory for strbldr");
		return NULL;
	}
	sb->buff = calloc(sizeof(char), INITIAL_BUFFSIZE);
	if (sb->buff == NULL) {
		error(AREA, "No memory for strbldr buff");
		return NULL;
	}
	sb->buffsize = INITIAL_BUFFSIZE;
	sb->buffidx = 0;
	sb->addc = addc;
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

static char *str(struct strbldr *sb)
{
	sb->buffidx = '\0';
	return sb->buff;
}

static char *copy(struct strbldr *sb)
{
	long slen;
	sb->buffidx = '\0';

	char *str = (char *)malloc((slen = (sb->buffidx + 1)) * sizeof(char));
	if (str == NULL) {
		error(AREA, "No memory to copy string");
		return NULL;
	}
	strcpy_s(str, slen, sb->buff);
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
		free(sb);
	}
}
