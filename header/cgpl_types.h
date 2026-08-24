#ifndef CGPL_INTERPRETER_TYPES_H_
#define CGPL_INTERPRETER_TYPES_H_

#include "ds/stringview.h"

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
    u32 line, col;
} Token;

#endif /* CGPL_INTERPRETER_TYPES_H_ */