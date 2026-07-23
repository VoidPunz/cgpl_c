#include "../header/cfg.h"

/* Func body */
CGPL_GRAMMAR_BUFFER _func[] = {TOKEN_SOF, GRAMMAR_INSTRUCTIONS, TOKEN_EOF};

/* Instructions */
CGPL_GRAMMAR_BUFFER _instructions_nonterminal1[] = {GRAMMAR_VARDECL};
CGPL_GRAMMAR_BUFFER _instructions_nonterminal2[] = {GRAMMAR_ASSIGN};

/* Whitespace */
CGPL_GRAMMAR_BUFFER _whitespace_terminal1[] = {TOKEN_WHITESPACE};
CGPL_GRAMMAR_BUFFER _whitespace_terminal2[] = {TOKEN_NEWLINE};
CGPL_GRAMMAR_BUFFER _whitespace_terminal3[] = {TOKEN_TAB};

/* Values */
CGPL_GRAMMAR_BUFFER _value_terminal1[] = {TOKEN_WORD};
CGPL_GRAMMAR_BUFFER _value_terminal2[] = {TOKEN_NUMERIC};

/* Vardecl */
CGPL_GRAMMAR_BUFFER _vardecl[] = {TOKEN_KEYWORD_VAR, GRAMMAR_WHITESPACE, TOKEN_WORD, GRAMMAR_WHITESPACE, GRAMMAR_VALUE};

/* Assign */
CGPL_GRAMMAR_BUFFER _assign[] = {TOKEN_WORD, GRAMMAR_WHITESPACE, GRAMMAR_VALUE};

const Grammar g_ContextFreeGrammar[GRAMMAR_LIMIT][GRAMMAR_CFG_MAX_SIZE] = {
    [GRAMMAR_FUNCBODY] = {
        {.type = GRAMMAR_FUNCBODY, .buffer = _func, .rep = 0},
        CGPL_GRAMMAR_DEFAULT(1)
    },
    [GRAMMAR_INSTRUCTIONS] = {
        {.type = GRAMMAR_INSTRUCTIONS, .buffer = _instructions_nonterminal1, .rep = 1},
        {.type = GRAMMAR_INSTRUCTIONS, .buffer = _instructions_nonterminal2, .rep = 1},
        CGPL_GRAMMAR_DEFAULT(2)
    },
    [GRAMMAR_WHITESPACE] = {
        {.type = GRAMMAR_WHITESPACE, .buffer = _whitespace_terminal1, .rep = 1},
        {.type = GRAMMAR_WHITESPACE, .buffer = _whitespace_terminal2, .rep = 1},
        {.type = GRAMMAR_WHITESPACE, .buffer = _whitespace_terminal3, .rep = 1},
        CGPL_GRAMMAR_DEFAULT(3)
    },
    [GRAMMAR_VALUE] = {
        {.type = GRAMMAR_VALUE, .buffer = _value_terminal1, .rep = 0},
        {.type = GRAMMAR_VALUE, .buffer = _value_terminal2, .rep = 0},
        CGPL_GRAMMAR_DEFAULT(2)
    },
    [GRAMMAR_VARDECL] = {
        {.type = GRAMMAR_VARDECL, .buffer = _vardecl, .rep = 0},
        CGPL_GRAMMAR_DEFAULT(1)
    },
    [GRAMMAR_ASSIGN] = {
        {.type = GRAMMAR_ASSIGN, .buffer = _assign, .rep = 0},
        CGPL_GRAMMAR_DEFAULT(1)
    },
};

const char* cgpl_grammar_tostring(grammar_t type) {
    switch (type) {
        case GRAMMAR_WHITESPACE:
            return "GRAMMAR_WHITESPACE";
        case GRAMMAR_VALUE:
            return "GRAMMAR_VALUE";
        case GRAMMAR_VARDECL:
            return "GRAMMAR_VARDECL";
        case GRAMMAR_ASSIGN:
            return "GRAMMAR_ASSIGN";
        case GRAMMAR_LIMIT:
        default:
            return "GRAMMAR_BAD";
    }
}