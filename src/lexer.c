#include "../header/lexer.h"

CGPL_KEYWORD_BUFFER _keyword_var[] = "var";

const KeywordTuple g_Keywords[] = {
    {.type = TOKEN_KEYWORD_VAR, .keyword = _keyword_var}
};

/* Checks if a lexeme is a digit (0-9)* */
static inline token_t is_digit(char ch) {
    return (ch >= '0' && ch <= '9') ? TOKEN_NUMERIC : TOKEN_NA;
}

/* Checks if a lexeme is an ASCII string ([A-Z][a-z])* */
static inline token_t is_ascii(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) ? TOKEN_ASCII : TOKEN_NA;
}

static token_t is_keyword(LexerState* ls, char ch) {
    static uint32_t size = 0;
    for (int i = 0; i < CGPL_ARRAY_SIZE(g_Keywords); i++) {
        const KeywordTuple* tuple = &g_Keywords[i];
        if (size > CGPL_ARRAY_SIZE(tuple->keyword) - 1) break;

        /* Ensure a numeric value does not precede the current word */
        if (is_digit(ls->prevCh) != TOKEN_NA) return TOKEN_WORD;

        int res = tuple->keyword[size++] == ch;

        /* Check if this is the first character in a new lexeme. We can assume that the type for 'ch' is TOKEN_WORD */
        #ifdef DEBUG
        if (ls->prevType != TOKEN_WORD && ls->prevType != tuple->type) {
            DEBUG_PRINT("New lexeme: %c\n", ch);
        } else
        #endif
        
        if (size == CGPL_ARRAY_SIZE(tuple->keyword) - 1) {
            DEBUG_PRINT("KEYWORD CHECK: %s, %c\n", tuple->keyword, ch);
            if (res) size = 0;
        }
        return res ? tuple->type : TOKEN_WORD;
    }
    return TOKEN_WORD;
}

/* Checks if a lexeme is a word ([A-Z][a-z])*([0-9])* */
static inline token_t is_word(LexerState* ls, char ch) {
    /* Control that if we see a digit, it's preceeded by an ascii or word token */
    if (is_digit(ch) == TOKEN_NUMERIC && ls->prevType == TOKEN_WORD) {
        return TOKEN_WORD;
    }
    /* Otherwise just check if char is an ASCII */
    return is_ascii(ch) == TOKEN_ASCII ? is_keyword(ls, ch) : TOKEN_NA;
}

/* Checks if a character is whitespace ' ' */
static inline token_t is_whitespace(char ch) {
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
static inline token_t is_operation(char ch) {
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

static inline List_Node* create_token_node(token_t type, char* lexemeBuffer, size_t size) {
    return list_new(cgpl_new_token(type, lexemeBuffer, size));
}

static char get_char(LexerState* ls) {
    char ch = '\0';
    if (ls->src.type == CGPL_SOURCE_FILE) {
        ch = fgetc(ls->src.fp);
    } else if (ls->src.type == CGPL_SOURCE_STRING) {
        if (ls->src.sbuffer.cursor >= ls->src.sbuffer.size) return EOF;
        ch = ls->src.sbuffer.ptr[ls->src.sbuffer.cursor];
        ls->src.sbuffer.cursor++;
    }
    return ch;
}

static int is_end(LexerState* ls) {
    if (ls->src.type == CGPL_SOURCE_FILE) {
        return file_peek(ls->src.fp) == EOF;
    } else if (ls->src.type == CGPL_SOURCE_STRING) {
        return ls->src.sbuffer.cursor == ls->src.sbuffer.size;
    }
    cgpl_error_fatal("Bad source type");
}

static void insert_node(LexerState* ls, char ch) {
    /* Create token and list node */
    token_t type = TOKEN_NA;
    List_Node* node = NULL;

    /* Lexically analyse the character, if none succeed throw a bad token error */
    if ((type = is_whitespace(ch)) == TOKEN_NA && (type = is_operation(ch)) == TOKEN_NA && (type = is_word(ls, ch)) == TOKEN_NA && (type = is_digit(ch)) == TOKEN_NA) {
        cgpl_error_fatal("Bad token");
    }

    /* CGPL_LEXEME_MAX_SIZE - 1 to guarantee final null terminator in string */
    if (ls->lexemeSize > CGPL_LEXEME_MAX_SIZE - 1) cgpl_error_fatal("Lexeme grew too large (Max %d characters)", CGPL_LEXEME_MAX_SIZE - 1);
    ls->lexemeBuffer[ls->lexemeSize++] = ch;

    DEBUG_PRINT("%c - %s | %s - %s\n", ch, ls->lexemeBuffer, cgpl_lexer_token_tostring(ls->prevType), cgpl_lexer_token_tostring(type));
    if (ls->prevType != TOKEN_NA && ls->prevType != type || is_end(ls)) {
        node = create_token_node(ls->prevType, ls->lexemeBuffer, ls->lexemeSize);
        if (ls->head == NULL) {
            ls->head = node;
        } else if (ls->tail == NULL) {
            ls->tail = node;
            list_connect(ls->head, node);
        } else {
            list_connect(ls->tail, node);
            ls->tail = node;
        }
        DEBUG_PRINT("Token created: %s, %s - %s\n", cgpl_lexer_token_tostring(ls->prevType), cgpl_lexer_token_tostring(type), ((Token*)node->data)->lexemeBuffer);
        if (ls->lexemeSize > 0) memset(ls->lexemeBuffer + 1, '\0', CGPL_LEXEME_MAX_SIZE - 1);
        ls->lexemeBuffer[0] = ch;
        ls->lexemeSize = 1;
    }

    ls->prevCh = ch;
    ls->prevType = type;
}

static void lexer_finish(LexerState* ls) {
    /* Create SOF node and replace as head node */
    List_Node* node = create_token_node(TOKEN_SOF, NULL, 0);
    list_connect(node, ls->head);
    ls->head = node;

    /* Create EOF node and replace as tail node */
    node = create_token_node(TOKEN_EOF, NULL, 0);
    list_connect(ls->tail, node);
    ls->tail = node;

    ls->status = LEXER_STATUS_FINISHED;
}

/* Perform an iteration at the cursor of the given source. */
static void cgpl_lexer_next(LexerState* ls) {
    /* New scope to pop all used variables off the stack before any recursion */
    {
        /* Read character on cursor from source */
        char ch = get_char(ls);
        if (ch == EOF) return lexer_finish(ls);
        insert_node(ls, ch);
    }

    cgpl_lexer_next(ls);
}

/* Initialize everything non-source related. */
static inline void init_state_base(LexerState* ls) {
    if (ls == NULL) ERROR_UNEXPECTED_NULL_PTR;
    memset(ls->lexemeBuffer, '\0', CGPL_LEXEME_MAX_SIZE);
    ls->lexemeSize = 0;
    ls->head = ls->tail = NULL;
    ls->status = LEXER_STATUS_READY;
    ls->prevType = TOKEN_NA;
    ls->prevCh = '\0';
}

Token* cgpl_new_token(token_t type, char* lexemeBuffer, size_t size) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) ERROR_BAD_ALLOC;
    token->type = type;
    if (size == 0 || lexemeBuffer == NULL) {
        memset(token->lexemeBuffer, '\0', CGPL_LEXEME_MAX_SIZE);
        token->size = 0;
    }
    else {
        memcpy(token->lexemeBuffer, lexemeBuffer, size);
        memset(token->lexemeBuffer + size, '\0', CGPL_LEXEME_MAX_SIZE - size);
        token->size = size;
    }
    return token;
}

void cgpl_lexer_init_state_file(LexerState *ls, FILE* fp) {
    if (fp == NULL) ERROR_UNEXPECTED_NULL_PTR;
    init_state_base(ls);
    ls->src.type = CGPL_SOURCE_FILE;
    ls->src.fp = fp;
}

void cgpl_lexer_init_state_string(LexerState* ls, char* ptr, size_t size) {
    if (ptr == NULL) ERROR_UNEXPECTED_NULL_PTR;
    init_state_base(ls);
    ls->src.type = CGPL_SOURCE_STRING;
    ls->src.sbuffer.ptr = ptr;
    ls->src.sbuffer.size = size;
    ls->src.sbuffer.cursor = 0;
}

void cgpl_lexer_tokenize(LexerState *ls) {
    if (ls->src.type < CGPL_SOURCE_FILE || ls->src.type > CGPL_SOURCE_STRING) cgpl_error_fatal("Bad source type");
    if (ls->status == LEXER_STATUS_FINISHED) {
        cgpl_warning("Attempted to tokenize a finished lexer state instance.");
        return;
    }

    /* Prevent a potential leak */
    if (ls->head != NULL) list_free_standalone_cascade(ls->head);
    ls->status = LEXER_STATUS_IN_PROGRESS;

    /* Begin */
    cgpl_lexer_next(ls);
}

const char* cgpl_lexer_token_tostring(const token_t type) {
    switch (type) {
        case TOKEN_WHITESPACE:
            return "TOKEN_WHITESPACE";
        case TOKEN_NEWLINE:
            return "TOKEN_NEWLINE";
        case TOKEN_TAB:
            return "TOKEN_TAB";
        case TOKEN_SOF:
            return "TOKEN_SOF";
        case TOKEN_NUMERIC:
            return "TOKEN_NUMERIC";
        case TOKEN_ASCII:
            return "TOKEN_ASCII";
        case TOKEN_WORD:
            return "TOKEN_WORD";
        case TOKEN_KEYWORD_VAR:
            return "TOKEN_KEYWORD_VAR";
        case TOKEN_EQUALS:
            return "TOKEN_EQUALS";
        case '+':
            return "TOKEN_CROSS";
        case '-':
            return "TOKEN_DASH";
        case '*':
            return "TOKEN_ASTERISK";
        case '/':
            return "TOKEN_FSLASH";
        case '%':
            return "TOKEN_PERCENT";
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_NA:
        default:
            return "TOKEN_NA";
    }
}

int file_peek(FILE* fp) {
    int ch = fgetc(fp);
    if (ch != EOF) ungetc(ch, fp);
    return ch;
}

const char* cgpl_lexer_print_token(const List_Node* node) {
    static char buffer[CGPL_LEXEME_MAX_SIZE] = {'\0'};
    if (node->data != NULL) {
        Token* token = (Token*)node->data;
        sprintf(buffer, "Token (%s): %s", cgpl_lexer_token_tostring(token->type), token->lexemeBuffer[0] == '\0' ? "???" : token->lexemeBuffer);
    } else {
        sprintf(buffer, "Token (%s): N/A", cgpl_lexer_token_tostring(TOKEN_NA));
    }
    return buffer;
}