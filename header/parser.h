#ifndef CGPL_PARSER_H_
#define CGPL_PARSER_H_

#include "arena/arena_include.h"
#include "ds/list.h"
#include "lexer.h"

#define SYNTAX_ERROR(msg) cgpl_error_fatal("Syntax error: %s", msg)
#define PARSER_PREFIX "[PARSER] "

#ifdef DEBUG
    #define DEBUG_PARSER(id, headNode, tailNode) DEBUG_PRINT("\t%s: [%s - %s (+%lld more)]\n", id, cgpl_token_tostring[get_token(headNode)->type], cgpl_token_tostring[get_token(*tailNode)->type], list_count(*tailNode))
#else
    #define DEBUG_PARSER(id, headNode, tailNode)
#endif

#define AST_KIND_LIST(X)\
    X(AST_SOF)          \
    X(AST_EOF)          \
    X(AST_INSTRUCTION)  \
    X(AST_STATEMENT)    \
    X(AST_FUNCTION)     \
    X(AST_VARDECL)      \
    X(AST_ASSIGN)       \
    X(AST_EXPRESSION)   \
    X(AST_TERM)         \
    X(AST_LIMIT)        \

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
Ast_Node* cgpl_new_ast(const Token* token, ast_kind_t kind);

#endif