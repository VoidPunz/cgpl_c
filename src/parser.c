#include "../header/parser.h"

#define EQ(node, value) (node != NULL && get_token(node)->type == value)
#define ATTEMPT(func, ast, resetNode, rest) if ((ast = func(rest)) == NULL) *rest = resetNode;

/* Returns the current token while advancing to the next in the list. */
static List_Node* next(List_Node** rest) {
    if (rest == NULL) return NULL;
    if (*rest == NULL) return NULL;
    List_Node* node = *rest;
    *rest = (*rest)->next;
    return node; 
}

/* Consumes whitespace until next non-whitespace token is found while ignoring newlines. Returns true if any whitespace has been removed. */
static bool consume(List_Node** rest) {
    DEBUG_PARSER("consume", rest);
    if (*rest == NULL) return false;
    const Token* token = get_token(*rest);
    bool isWhitespace = IS_WHITESPACE(token->type) && token->type != TOKEN_NEWLINE;
    if (isWhitespace) {
        next(rest);
        return consume(rest);
    }
    return false;
}

/* Peek n number of tokens ahead (ignores whitespace) */
static token_t peek(List_Node* node, size_t n) {
    while (n-- != 0 && node != NULL) {
        consume(&node);
        node = node->next;
    }
    return node != NULL ? get_token(node)->type : TOKEN_NA;
}

static Ast_Node* statement(List_Node** rest) {
    DEBUG_PARSER("statement", rest);
    return NULL;
}

/* _value_ */
static Ast_Node* value(List_Node** rest) {
    consume(rest);
    DEBUG_PARSER("value", rest);
    if (!(EQ(*rest, TOKEN_NUMERIC) || EQ(*rest, TOKEN_KEYWORD_BOOL))) SYNTAX_ERROR("Attempted to assign a symbol to a non-value.");
    Ast_Node* ast = cgpl_ast_new(get_token(next(rest)), CGPL_AST_VALUE);
    return ast;
}

/* _id_ | = _value_ */
static Ast_Node* assignment(List_Node** rest) {
    consume(rest);
    DEBUG_PARSER("assignment", rest);
    if (!EQ(*rest, TOKEN_EQUALS)) SYNTAX_ERROR("Expected a '=' after identifier.");
    Ast_Node* ast = cgpl_ast_new(get_token(next(rest)), CGPL_AST_ASSIGN);
    ast->treeNode = list_new(value(rest));
    return ast;
}

/* var | _id_ = _value_ */
static Ast_Node* vardecl(List_Node** rest) {
    consume(rest);
    DEBUG_PARSER("vardecl", rest);
    if (!EQ(*rest, TOKEN_WORD)) SYNTAX_ERROR("Expected a word after 'var' keyword.");
    Ast_Node* ast = cgpl_ast_new(get_token(next(rest)), CGPL_AST_VARDECL);
    ast->treeNode = list_new(assignment(rest));
    return ast;
}

static Ast_Node* instruction(List_Node** rest) {
    DEBUG_PARSER("instruction", rest);
    Ast_Node* ast = NULL;

    if (EQ(*rest, TOKEN_KEYWORD_VAR)) {
        ast = cgpl_ast_new(get_token(next(rest)), CGPL_AST_INSTRUCTION);
        ast->treeNode = list_new(vardecl(rest));
    }
    return ast;
}

/* Dynamically attempt to parse either a statement or an instruction. */
static Ast_Node* start(List_Node** rest) {
    DEBUG_PARSER("start", rest);

    /* Look for either a statement or instruction */
    Ast_Node* ast = NULL;

    if (!EQ(*rest, TOKEN_SOF)) SYNTAX_ERROR("Missing SOF token");
    next(rest);
    List_Node* resetNode = *rest;
    ATTEMPT(statement, ast, resetNode, rest)
    ATTEMPT(instruction, ast, resetNode, rest)
    if (ast == NULL) cgpl_error_fatal("Failed to parse neither a statement nor an instruction.");
    consume(rest);
    if (!EQ(*rest, TOKEN_EOF)) SYNTAX_ERROR("Missing EOF token");

    return ast;
}

Ast_Node* cgpl_parse(List_Node* tokenNode) {
    return start(&tokenNode);
}

Ast_Node* cgpl_ast_new(const Token* token, ast_kind_t kind) {
    Ast_Node* node = (Ast_Node*)malloc(sizeof(Ast_Node));
    node->token = token;
    node->treeNode = NULL;
    node->kind = kind;
    return node;
}

static inline void cgpl_ast_print_internal(Ast_Node* ast, size_t c) {
    for (size_t i = 0; i < c; i++) putchar('\t');
    DEBUG_PRINT("%s: %s\n", ast_kind_tostring[ast->kind], cgpl_token_tostring[ast->token->type]);
}

void cgpl_ast_print(Ast_Node* ast) {
    static size_t c = 0;
    List_Node* node = ast->treeNode;
    printf("\n");
    cgpl_ast_print_internal(ast, c);
    while (node != NULL) {
        cgpl_ast_print_internal((Ast_Node*)node->data, c++);
        cgpl_ast_print((Ast_Node*)node->data);
        node = node->next;
    }
}