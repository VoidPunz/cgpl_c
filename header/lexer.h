#ifndef CGPL_LEXER_H_
#define CGPL_LEXER_H_

#include <stdlib.h>
#include <string.h>
#include "ds/list.h"
#include "types.h"
#include "debug.h"

#define CGPL_LEXEME_MAX_SIZE 64
#define CGPL_KEYWORD_BUFFER static const char
#define IS_TOKEN(n) (n >= TOKEN_SOF && n < TOKEN_LIMIT)
#define IS_WHITESPACE(n) (n >= TOKEN_WHITESPACE && n <= TOKEN_TAB)

#ifdef DEBUG
   /* Printer function for lists using tokens as data */
    const char* cgpl_lexer_print_token(const ListNode* node);
#endif

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
                u64 size;
                /* Current cursor position on the string source. */
                u64 cursor;
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
    u64 lexemeSize;
    /* The current line */
    u64 line;
    /* Column of the line */
    u64 col;
    /* Head node of the list of tokens. This will always be an SOF token. */
    ListNode *head;
    /* Tail node of the list of tokens. */
    ListNode* tail;
    /* The token type yielded from the previous iteration. */
    token_t prevType;
    /* The previous iteration's character. */
    char prevCh;
} LexerState;

/* Tuple structure for keywords */
typedef struct {
    /* Keyword string */
    const StringView keyword;
    /* Type for the associated keyword */
    token_t type;
} KeywordTuple;

/* Array of all the keywords in CGPL */
extern const KeywordTuple g_Keywords[];
/* Allocate a new token with a given type */
Token* cgpl_new_token(token_t type, char* lexemeBuffer, u64 size, u32 line, u32 col);
/* Initialize a lexer state with a file source. */
void cgpl_lexer_init_state_file(LexerState* ls, FILE* fp);
/* Initialize a lexer state with a string source. */
void cgpl_lexer_init_state_string(LexerState* ls, char* sp, u64 ss);
/* Tokenizes a source (either a file with .cgpl extension or raw string) and returns a list of tokens. */
ListNode* cgpl_lexer_tokenize(char* source);
/* Shorthand for grabbing the token value from a node */
static inline Token* get_token(ListNode* node) {
    return (Token*)node->data;
}

#endif /* CGPL_LEXER_H_ */