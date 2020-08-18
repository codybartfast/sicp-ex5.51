#ifndef CHARACTER_H
#define CHARACTER_H
#include "sicpstd.h"

#include <stdbool.h>

bool is_delimiter(char c);
bool is_digit(char c);
bool is_initial(char c);
bool is_letter(char c); // *
bool is_peculiar_identifier(char c);
bool is_special_initial(char c); // *
bool is_subsequent(char c);
bool is_special_subsequent(char c); // *
bool is_whitespace(char c);

//* No currently used
#endif
