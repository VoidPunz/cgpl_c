#ifndef CGPL_PARSER_H_
#define CGPL_PARSER_H

#include "list.h"
#include "lexer.h"

#define SYNTAX_ERROR(msg) cgpl_error_fatal("Syntax error: %s", msg)

#ifdef DEBUG
    #define DEBUG_PARSER(id, rest, node) DEBUG_PRINT("%s: [rest: %s - %d, node: %s - %d]\n", id, cgpl_token_tostring[get_token(*rest)->type], list_count(*rest), cgpl_token_tostring[get_token(node)->type], list_count(node))
#else
    #define DEBUG_PARSER(id, rest, node)
#endif

/* X-macro for AST kinds */
#define AST_KIND_LIST(X)    \
    X(CGPL_AST_VARDECL)     \
    X(CGPL_AST_ASSIGN)      \
    X(CGPL_AST_VALUE)       \
    X(CGPL_AST_LIMIT)       \

typedef enum {
    AST_KIND_LIST(GENERATE_ENUM)
} ast_kind_t;

static const char* ast_kind_tostring[] = {
    AST_KIND_LIST(GENERATE_STRING)
};

typedef struct Ast_Node {
    /* Pointer to the associated token (which may contain any semantic information) relevant to this AST kind */
    const Token* token;
    /* The kind of this AST */
    ast_kind_t kind; 
    /* Tree node of Ast_Nodes whose nodes contain tokens*/
    List_Node* treeNode;
} Ast_Node;

/* Parse a list of tokens into an abstract syntax tree. */
Ast_Node* cgpl_parse(List_Node* tokenNode);
/* Allocate a new AST node on the heap. */
Ast_Node* cgpl_ast_new(const Token* token, ast_kind_t kind);
/* Prints the AST tree. */
void cgpl_ast_print(Ast_Node* ast);

#endif