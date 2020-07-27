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

source *source_file(FILE *);
source *source_name(char *);
source *source_string(char *);

int source_close(source *);

int sgetc(source *);
