#ifndef CGPL_TYPES_H_
#define CGPL_TYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

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

#endif /* CGPL_TYPES_H_ */