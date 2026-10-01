#include "../header/lexer.h"

const KeywordTuple g_Keywords[] = {
    {.type = TOKEN_KEYWORD_VAR, .keyword = STRINGVIEW("var")},
    {.type = TOKEN_KEYWORD_BOOL, .keyword = STRINGVIEW("true")},
    {.type = TOKEN_KEYWORD_BOOL, .keyword = STRINGVIEW("false")},
};

/* Checks if a lexeme is a digit (0-9)* */
ALWAYS_INLINE token_t is_digit(char ch) {
    return (ch >= '0' && ch <= '9') ? TOKEN_NUMERIC : TOKEN_NA;
}

/* Checks if a lexeme is an ASCII string ([A-Z][a-z])* */
ALWAYS_INLINE token_t is_ascii(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) ? TOKEN_ASCII : TOKEN_NA;
}

/* Check if a lexeme is part of a keyword. This function assumes the current type of the given character is of TOKEN_ASCII. */
static token_t is_keyword(LexerState* ls, char ch) {
    static u32 size = 0;
    for (u64 i = 0; i < CGPL_ARRAY_SIZE(g_Keywords); i++) {
        const KeywordTuple* tuple = &g_Keywords[i];
        if (size > sizeof(tuple->keyword.cstr) - 1) break;

        // Ensure a numeric value does not precede the current word
        if (is_digit(ls->prevCh) != TOKEN_NA) return TOKEN_WORD;

        bool res = tuple->keyword.cstr[size++] == ch;

        // Check if this is the first character in a new lexeme. We can assume that the type for 'ch' is TOKEN_WORD
        #ifdef DEBUG
        if (ls->prevType != TOKEN_WORD && ls->prevType != tuple->type) {
            DEBUG_PRINT("New lexeme: %c\n", ch);
        } else
        #endif
        
        if (size == sizeof(tuple->keyword.cstr) - 1) {
            DEBUG_PRINT("KEYWORD CHECK: %s, %c\n", tuple->keyword.cstr, ch);
            if (res) size = 0;
        }
        return res ? tuple->type : TOKEN_WORD;
    }
    return TOKEN_WORD;
}

/* Checks if a lexeme is a word ([A-Z][a-z])*([0-9])* */
ALWAYS_INLINE token_t is_word(LexerState* ls, char ch) {
    // Control that if we see a digit, it's preceeded by an ascii or word token
    if (is_digit(ch) == TOKEN_NUMERIC && ls->prevType == TOKEN_WORD) {
        return TOKEN_WORD;
    }
    // Otherwise just check if char is an ASCII and possibly a keyword
    return is_ascii(ch) == TOKEN_ASCII ? is_keyword(ls, ch) : TOKEN_NA;
}

/* Checks if a character is whitespace ' ' */
ALWAYS_INLINE token_t is_whitespace(char ch) {
    switch (ch) {
        case ' ':
            return TOKEN_WHITESPACE;
        case '\n':
            return TOKEN_NEWLINE;
        case '\t':
            return TOKEN_TAB;
        default:
            return TOKEN_NA;
    }
}

/* Checks if a lexeme is an operation. */
ALWAYS_INLINE token_t is_operation(char ch) {
    switch (ch) {
        case '=':
            return TOKEN_EQUALS;
        case '+':
            return TOKEN_CROSS;
        case '-':
            return TOKEN_DASH;
        case '*':
            return TOKEN_ASTERISK;
        case '/':
            return TOKEN_FSLASH;
        case '%':
            return TOKEN_PERCENT;
        default:
            return TOKEN_NA;
    }
}

ALWAYS_INLINE ListNode* create_token_node(token_t type, char* lexemeBuffer, u64 size, u32 line, u32 col) {
    return list_new(cgpl_new_token(type, lexemeBuffer, size, line, col));
}

static char get_char(LexerState* ls) {
    char ch = '\0';
    if (ls->sourceCursor >= ls->sourceSize) return EOF;
    ch = ls->sourceBuffer[ls->sourceCursor];
    ls->sourceCursor++;
    return ch;
}

ALWAYS_INLINE int is_end(LexerState* ls) {
    return ls->sourceCursor >= ls->sourceSize;
}

static void insert_node(LexerState* ls, char ch) {
    // Create token and list node
    token_t type = TOKEN_NA;
    ListNode* node = NULL;

    // Lexically analyse the character, if none succeed throw a bad token error
    if ((type = is_whitespace(ch)) == TOKEN_NA && (type = is_operation(ch)) == TOKEN_NA && (type = is_word(ls, ch)) == TOKEN_NA && (type = is_digit(ch)) == TOKEN_NA) {
        cgpl_error_fatal("Bad token: (line: %lld, col: %lld) \"%c\"", ls->line, ls->col, ch);
    }

    if (ls->lexemeSize + 1 >= CGPL_LEXEME_MAX_SIZE) cgpl_error_fatal("Lexeme grew too large (Max %d characters)", CGPL_LEXEME_MAX_SIZE);
    ls->lexemeBuffer[ls->lexemeSize++] = ch;

    DEBUG_PRINT("%c - %s | %s - %s\n", ch, ls->lexemeBuffer, cgpl_token_tostring[ls->prevType], cgpl_token_tostring[type]);
    if ((ls->prevType != TOKEN_NA && ls->prevType != type) || is_end(ls)) {
        node = create_token_node(ls->prevType, ls->lexemeBuffer, ls->lexemeSize, ls->line, ls->col);
        if (ls->head == NULL) {
            ls->head = node;
        } else if (ls->tail == NULL) {
            ls->tail = node;
            list_connect(ls->head, node);
        } else {
            list_connect(ls->tail, node);
            ls->tail = node;
        }
        DEBUG_PRINT("Token created: %s, %s\n", cgpl_token_tostring[ls->prevType], cgpl_token_tostring[type]);
        if (ls->lexemeSize > 0) memset(ls->lexemeBuffer + 1, '\0', CGPL_LEXEME_MAX_SIZE - 1);
        ls->lexemeBuffer[0] = ch;
        ls->lexemeSize = 1;
    }

    if (type == TOKEN_NEWLINE) {
        ls->line++;
        ls->col = 0;
    } else {
        ls->col++;
    }

    ls->prevCh = ch;
    ls->prevType = type;
}

static void lexer_finish(LexerState* ls) {
    const u64 size = 0;
    const u32 line = 0, col = 0;
    
    // Create SOF node and replace as head node
    ListNode* node = create_token_node(TOKEN_SOF, NULL, size, line, col);
    list_connect(node, ls->head);
    ls->head = node;

    // Create EOF node and replace as tail node
    node = create_token_node(TOKEN_EOF, NULL, size, line, col);
    list_connect(ls->tail, node);
    ls->tail = node;
}

// TODO: Rewrite as an iterative loop instead of recursion
/* Perform an iteration at the cursor of the given source. */
static void cgpl_lexer_next(LexerState* ls) {
    // New scope to pop all used variables off the stack before any recursion
    {
        // Read character on cursor from source
        char ch = get_char(ls);
        if (ch == EOF) return lexer_finish(ls);
        insert_node(ls, ch);
    }

    cgpl_lexer_next(ls);
}

/* Initialize everything non-source related. */
static void init_state_base(LexerState* ls) {
    if (ls == NULL) ERROR_UNEXPECTED_NULL_PTR;
    memset(ls->lexemeBuffer, '\0', CGPL_LEXEME_MAX_SIZE);
    ls->lexemeSize = ls->line = ls->col = 0;
    ls->head = ls->tail = NULL;
    ls->prevType = TOKEN_NA;
    ls->prevCh = '\0';
}

Token* cgpl_new_token(token_t type, char* lexemeBuffer, u64 size, u32 line, u32 col) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) ERROR_BAD_ALLOC;
    token->type = type;
    token->line = line;
    token->col = col;
    switch (type) {
        case TOKEN_NUMERIC:
            char* end;
            token->sem.num = strtod(lexemeBuffer, &end);
            if (lexemeBuffer == end) cgpl_error_fatal("Bad double value");
            break;
        case TOKEN_WORD:
            // TODO
            break;
        case TOKEN_KEYWORD_BOOL:
            // TODO
        default:
            break;
    }
    return token;
}

/* Reads a file with known size in filesystem into a character buffer in RAM. Returns size of the file and the buffer if successful. */
static u64 read_file(char* source, char** buffer) {
    struct stat st;
    u64 size = 0;
    if (stat(source, &st) == 0) {
        size = st.st_size;
    }
    /* TODO: Push to error stack */
    else return 0;
    /* TODO: Push to error stack */
    if (size == 0) return 0;
    *buffer = (char*)malloc(size + 1);
    if (*buffer == NULL) ERROR_BAD_ALLOC
    FILE* fp = fopen(source, "r");
    u64 bytesRead = fread(buffer, 1, size, fp);
    fclose(fp);
    *buffer[bytesRead] = '\0';
    return size;
}

u64 cgpl_lexer_init_state_file(LexerState* ls, char* fs) {
    char* buffer = NULL;
    u64 size = read_file(fs, &buffer);
    cgpl_lexer_init_state_string(ls, buffer, size);
    return size;
}

void cgpl_lexer_init_state_string(LexerState* ls, char* ptr, u64 size) {
    if (ptr == NULL) ERROR_UNEXPECTED_NULL_PTR
    init_state_base(ls);
    ls->sourceBuffer = ptr;
    ls->sourceSize = size;
    ls->sourceCursor = 0;
}

#define CGPL_EXTENSION ".cgpl"
static bool is_cgpl_file(const char *path)
{
    const char* extension = strrchr(path, '.');
    if (extension == NULL) return false;
    return strcmp(extension, CGPL_EXTENSION) == 0;
}

ListNode* cgpl_lexer_tokenize(char* source) { 
    LexerState ls;

    if (is_cgpl_file(source)) {
        cgpl_lexer_init_state_file(&ls, source);
    } else {
        cgpl_lexer_init_state_string(&ls, source, strlen(source));
    }
    // TODO: Push to error stack
    if (ls.sourceBuffer == NULL || ls.sourceSize == 0) return NULL;

    // Begin
    cgpl_lexer_next(&ls);
    return ls.head;
}

#ifdef DEBUG
    const char* cgpl_lexer_print_token(const ListNode* node) {
        static char buffer[CGPL_LEXEME_MAX_SIZE] = {'\0'};
        if (node->data != NULL) {
            const Token* token = (Token*)node->data;
            sprintf(buffer, "Token (%s)", cgpl_token_tostring[token->type]);
        } else {
            sprintf(buffer, "Token (%s)", cgpl_token_tostring[TOKEN_NA]);
        }
        return buffer;
    }
#endif