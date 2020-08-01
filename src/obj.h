#ifndef OBJ_H
#define OBJ_H 1

#include <stdbool.h>
#include <stdint.h>

#define TYPE_NOT_SET 0
#define TYPE_NUMBER 2
#define TYPE_STRING 3
#define TYPE_EOF 4
#define TYPE_ERROR 5

union value {
	int64_t int64;
	char *string;
};

typedef struct {
	uint8_t type;
	uint8_t subtype;
	union value val;
} obj;

// NUMBER

#define NUMBER_INT64 1

bool isnumber(obj *);
obj *int64(int64_t);

// STRING
bool isstring(obj *);
obj *string(char *);

// EOF
extern obj *eof;
bool iseof(obj *obj);

// ERROR

#define ERROR_MEMORY 1
#define ERROR_INTERNAL 2
#define ERROR_LEXOR 3
#define ERROR_PARSER 4
#define ERROR_CONVERT 5
#define ERROR_EVAL 6

bool iserr(obj *obj);
obj *error_memory(void);
obj *error_internal(void);
obj *error_lexor(void);
obj *error_parser(void);
obj *error_convert(void);
obj *error_eval(void);

#endif
