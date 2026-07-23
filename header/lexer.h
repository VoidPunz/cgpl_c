#ifndef CGPL_LEXER_H_
#define CGPL_LEXER_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "list.h"
#include "types.h"
#include "debug.h"

#define CGPL_LEXEME_MAX_SIZE 65 // + 1 for '\0'
#define CGPL_KEYWORD_BUFFER static const char
#define IS_TOKEN(n) (n >= TOKEN_SOF && n < TOKEN_LIMIT)

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

typedef struct {
    char lexemeBuffer[CGPL_LEXEME_MAX_SIZE];
    token_t type;
    uint32_t size;
} Token;

typedef enum {
    LEXER_STATUS_READY = 0,
    LEXER_STATUS_IN_PROGRESS,
    LEXER_STATUS_FINISHED,
    LEXER_STATUS_LIMIT
} lexer_status;

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
    /* Temporary lexeme buffer */
    char lexemeBuffer[CGPL_LEXEME_MAX_SIZE];
    /* Currnent size of the lexeme buffer */
    size_t lexemeSize;
    /* Head node of the list of tokens. This will always be an SOF token. */
    List_Node *head;
    /* Tail node of the list of tokens. */
    List_Node* tail;
    /* The current status of the lexer state. */
    lexer_status status;
    /* The token type yielded from the previous iteration. */
    token_t prevType;
    /* The previous iteration's character. */
    char prevCh;
} LexerState;

/* Tuple structure for keywords */
typedef struct {
    /* Keyword string */
    const char* keyword;
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
/* Tokenizes a ready lexer state by modifying the provided pointer. See the results using ls->status and ls->head. Do NOT call list_free_cascading with the list nodes stored in the structure, use list_free_standalone_cascade instead, as the data are just enum values stored as void pointers. */
void cgpl_lexer_tokenize(LexerState* ls);
/* Converts a token type to a string */
const char* cgpl_lexer_token_tostring(const token_t type);
/* Peek the next character in the file. */
int file_peek(FILE* fp);
/* Printer function for lists using tokens as data */
const char* cgpl_lexer_print_token(const List_Node* node);

#endif /* CGPL_LEXER_H_ */