#ifndef CGPL_LEXER_H_
#define CGPL_LEXER_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stringview.h"
#include "utils.h"
#include "list.h"
#include "types.h"
#include "debug.h"

#define CGPL_LEXEME_MAX_SIZE 64
#define CGPL_KEYWORD_BUFFER static const char
#define IS_TOKEN(n) (n >= TOKEN_SOF && n < TOKEN_LIMIT)
#define IS_WHITESPACE(n) (n >= TOKEN_WHITESPACE && n <= TOKEN_TAB)

typedef enum {
    /* Non-applicable */
    TOKEN_NA = 0x40,
    /* Start of function */
    TOKEN_SOF,
    /* Whitespace ' ' */
    TOKEN_WHITESPACE,
    /* Newline '\n' */
    TOKEN_NEWLINE,
    /* Tab \t */
    TOKEN_TAB,
    /* A numeric value (0-9)* */
    TOKEN_NUMERIC,
    /* ASCII characters ([A-Z][a-z])* */
    TOKEN_ASCII,
    /* ([A-Z][a-z])*([0-9])* */
    TOKEN_WORD,
    /* A boolean value (keywords true/false)*/
    TOKEN_KEYWORD_BOOL,
    /* 'var' keyword */
    TOKEN_KEYWORD_VAR,
    /* '=' */
    TOKEN_EQUALS,
    /* '+' */
    TOKEN_CROSS,
    /* '-' */
    TOKEN_DASH,
    /* '*' */
    TOKEN_ASTERISK,
    /* '/' */
    TOKEN_FSLASH,
    /* '%' */
    TOKEN_PERCENT,
    /* End of function */
    TOKEN_EOF,
    TOKEN_LIMIT
} token_t;

typedef union {
    cgpl_number_t num;
    cgpl_string_t word;
    cgpl_bool_t b;
} SemanticValue;

typedef struct {
    token_t type;
    SemanticValue sem;
} Token;

/* Lexer state object. Use either cgpl_lexer_init_state_file or cgpl_lexer_init_state_string to initialize depending on the corresponding source. */
typedef struct {
    /* Source info */
    struct {
        union {
            /* File source */
            FILE* fp;
            /* String buffer source */
            struct {
                /* Source */
                char* ptr;
                /* Size of source */
                size_t size;
                /* Current cursor position on the string source. */
                size_t cursor;
            } sbuffer;
        };
        enum {
            CGPL_SOURCE_FILE = 0,
            CGPL_SOURCE_STRING,
            CGPL_SOURCE_LIMIT
        } type;
    } src;
    /* Temporary lexeme */
    char lexemeBuffer[CGPL_LEXEME_MAX_SIZE];
    /* Lexeme size */
    size_t lexemeSize;
    /* The current line */
    size_t line;
    /* Column of the line */
    size_t col;
    /* Head node of the list of tokens. This will always be an SOF token. */
    List_Node *head;
    /* Tail node of the list of tokens. */
    List_Node* tail;
    /* The token type yielded from the previous iteration. */
    token_t prevType;
    /* The previous iteration's character. */
    char prevCh;
} LexerState;

/* Tuple structure for keywords */
typedef struct {
    /* Keyword string */
    const String_View keyword;
    /* Type for the associated keyword */
    token_t type;
} KeywordTuple;

/* Array of all the keywords in CGPL */
extern const KeywordTuple g_Keywords[];
/* Allocate a new token with a given type */
Token* cgpl_new_token(token_t type, char* lexemeBuffer, size_t size);
/* Initialize a lexer state with a file source. */
void cgpl_lexer_init_state_file(LexerState* ls, FILE* fp);
/* Initialize a lexer state with a string source. */
void cgpl_lexer_init_state_string(LexerState* ls, char* sp, size_t ss);
/* Tokenizes a source (either a file with .cgpl extension or raw string) and returns a list of tokens. */
List_Node* cgpl_lexer_tokenize(char* source);
/* Converts a token type to a string */
const char* cgpl_lexer_token_tostring(const token_t type);
/* Peek the next character in the file. */
int file_peek(FILE* fp);
/* Printer function for lists using tokens as data */
const char* cgpl_lexer_print_token(const List_Node* node);
/* Shorthand for grabbing the token value from a node */
static inline Token* get_token(List_Node* node) {
    return (Token*)node->data;
}

#endif /* CGPL_LEXER_H_ */