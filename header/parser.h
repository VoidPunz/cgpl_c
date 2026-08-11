#ifndef CGPL_PARSER_H_
#define CGPL_PARSER_H_

#include "ds/list.h"
#include "lexer.h"

#define SYNTAX_ERROR(msg) cgpl_error_fatal("Syntax error: %s", msg)

#ifdef DEBUG
    #define DEBUG_PARSER(id, rest) DEBUG_PRINT("%s: [rest: %s - %d]\n", id, cgpl_token_tostring[get_token(*rest)->type], list_count(*rest))
#else
    #define DEBUG_PARSER(id, rest)
#endif

#define AST_KIND_LIST(X)    \
    X(CGPL_AST_INSTRUCTION) \
    X(CGPL_AST_VARDECL)     \
    X(CGPL_AST_ASSIGN)      \
    X(CGPL_AST_VALUE)       \
    X(CGPL_AST_LIMIT)       \

typedef enum {
    AST_KIND_LIST(GENERATE_ENUM)
} ast_kind_t;

typedef struct Ast_Node {
    /* Pointer to the associated token (which may contain any semantic information) relevant to this AST kind */
    const Token* token;
    /* The kind of this AST */
    ast_kind_t kind; 
    /* Tree node of Ast_Nodes whose nodes contain tokens*/
    ListNode* treeNode;
} Ast_Node;

#ifdef DEBUG
    static const char* ast_kind_tostring[] = {
        AST_KIND_LIST(GENERATE_STRING)
    };

    /* Prints the AST tree. */
    void cgpl_ast_print(Ast_Node* ast);
#endif

/* Parse a list of tokens into an abstract syntax tree. */
Ast_Node* cgpl_parse(ListNode* tokenNode);
/* Allocate a new AST node on the heap. */
Ast_Node* cgpl_ast_new(const Token* token, ast_kind_t kind);

#endif