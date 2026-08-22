#ifndef CGPL_TYPES_H_
#define CGPL_TYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include "ds/stringview.h"
#include "utils.h"

#define TOKEN_TYPE_LIST(X)  \
    X(TOKEN_NA)             \
    X(TOKEN_SOF)            \
    X(TOKEN_WHITESPACE)     \
    X(TOKEN_NEWLINE)        \
    X(TOKEN_TAB)            \
    X(TOKEN_NUMERIC)        \
    X(TOKEN_ASCII)          \
    X(TOKEN_WORD)           \
    X(TOKEN_KEYWORD_BOOL)   \
    X(TOKEN_KEYWORD_VAR)    \
    X(TOKEN_EQUALS)         \
    X(TOKEN_CROSS)          \
    X(TOKEN_DASH)           \
    X(TOKEN_ASTERISK)       \
    X(TOKEN_FSLASH)         \
    X(TOKEN_PERCENT)        \
    X(TOKEN_EOF)            \
    X(TOKEN_LIMIT)          \

typedef enum {
    TOKEN_TYPE_LIST(GENERATE_ENUM)
} token_t;

static const char* cgpl_token_tostring[] = {
    TOKEN_TYPE_LIST(GENERATE_STRING)
};

typedef double cgpl_number_t;
typedef StringView cgpl_string_t;
typedef bool cgpl_bool_t;

typedef union {
    cgpl_number_t num;
    cgpl_string_t word;
    cgpl_bool_t b;
} SemanticValue;

typedef struct {
    SemanticValue sem;
    token_t type;
    uint32_t line, col;
} Token;

#endif /* CGPL_TYPES_H_ */