#include <stdio.h>

enum source_kind
{
    SOURCE_FILE_POINTER,
    SOURCE_FILE_NAME,
    SOURCE_STRING
};

typedef struct
{
    enum source_kind kind;
    char *name;
    FILE *file;
    char *text;
    char *next;
} source;

source *sopen_file(FILE *);
source *sopen_name(char *);
source *sopen_string(char *);

int sclose(source *);

int sgetc(source *);
