#ifndef CGPL_CFG_H_
#define CGPL_CFG_H_

#include "lexer.h"

#define GRAMMAR_CFG_MAX_SIZE 8
#define CGPL_GRAMMAR_DEFAULT(offset) [offset ... (GRAMMAR_CFG_MAX_SIZE - 1)] = {.buffer = NULL, .type = GRAMMAR_LIMIT, .rep = 0}
#define CGPL_GRAMMAR_BUFFER static const int
#define IS_CFG(n) (n >= GRAMMAR_FUNCBODY && n < GRAMMAR_LIMIT)

typedef enum {
    /* Function body */
    GRAMMAR_FUNCBODY,
    /* Any type of instructions */
    GRAMMAR_INSTRUCTIONS,
    /* \t\r\n*/
    GRAMMAR_WHITESPACE,
    /* String, numeric or ... */
    GRAMMAR_VALUE,
    /* A declaration of a variable */
    GRAMMAR_VARDECL,
    /* Assignment of a variable to a value */
    GRAMMAR_ASSIGN,
    GRAMMAR_LIMIT
} grammar_t;

/* Grammar structure for context free grammar */
typedef struct {
    /* Buffer of token_t/grammar_t representing a CFG for this grammar type */
    const int* buffer;
    /* The CFG type for this grammar */
    grammar_t type;
    /* Boolean: Are repitions of this pattern allowed? */
    int rep;
} Grammar;

/* Global context-free-grammar for CGPL */
extern const Grammar g_ContextFreeGrammar[GRAMMAR_LIMIT][GRAMMAR_CFG_MAX_SIZE];
/* Converts a grammar type to a string */
const char* cgpl_grammar_tostring(grammar_t type);

#endif /* CGPL_CFG_H_ */