#include "strbldr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define AREA "strbldr"
#define INITIAL_BUFFSIZE (1 << 0)

static bool grow_buff(struct strbldr *);

char *err_msg = "*** strbldr failed to allocate memory ***";

int sb_addc(struct strbldr *sb, char c)
{
	if (sb->errored)
		return EOF;
	if (sb->buffidx >= sb->buffsize - 1) {
		if (!grow_buff(sb))
			return EOF;
	}
	return sb->buff[sb->buffidx++] = c;
}

int sb_adds(struct strbldr *sb, const char *s)
{
	int rc = 0;

	if (sb->errored)
		return EOF;
	if (s == NULL)
		return EOF;
	while (*s != '\0' && rc != EOF)
		rc = sb_addc(sb, *s++);
	return (rc == EOF) ? EOF : 0;
}

char *sb_string(struct strbldr *sb)
{
	if (sb->errored)
		return err_msg;
	sb->buff[sb->buffidx] = '\0';
	return sb->buff;
}

char *sb_copy(struct strbldr *sb)
{
	char *str;

	if (sb->errored)
		return err_msg;
	sb->buff[sb->buffidx] = '\0';
	str = (char *)malloc((sb->buffidx + 1) * sizeof(char));
	if (str == NULL) {
		eprintf(AREA, "No memory to copy strbldr string");
		return NULL;
	}
	strcpy(str, sb->buff);
	return str;
}

struct strbldr *sb_clear(struct strbldr *sb)
{
	if (sb != NULL)
		sb->buffidx = 0;
	return sb;
}

static bool grow_buff(struct strbldr *sb)
{
	int i;
	char *new_buff;
	int new_buffsize;

	new_buffsize = 2 * sb->buffsize;
	new_buff = (char *)calloc(sizeof(char), new_buffsize);
	if (new_buff == NULL) {
		sb->errored = true;
		return false;
	}
	for (i = 0; i < sb->buffidx; i++)
		new_buff[i] = sb->buff[i];
	free(sb->buff);
	sb->buff = new_buff;
	sb->buffsize = new_buffsize;
	return true;
}

void sb_free(struct strbldr **sbref)
{
	struct strbldr *sb = *sbref;
	if (sb != NULL) {
		free(sb->buff);
		sb->buff = NULL;
		free(sb);
	}
	*sbref = NULL;
}

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
	sb->errored = false;
	return sb;
}
