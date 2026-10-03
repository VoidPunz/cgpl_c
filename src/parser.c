#include "../header/parser.h"

#define PARSE_FUNC static Ast_Node*

/* Attempt to parse using a given parser function. If the out is NULL, the tailNode gets set to the resetNode. */
ALWAYS_INLINE bool attempt(Ast_Node* (*parse_func)(ListNode*, ListNode**), Ast_Node** out, ListNode* headNode, ListNode* resetNode, ListNode** tailNode) {
    DEBUG_PRINT("ATTEMPT BEGIN...\n");
    bool hasFailed = (*out = parse_func(headNode, tailNode)) == NULL;
    if (hasFailed) {
        *tailNode = resetNode;
        DEBUG_PRINT("ATTEMPT FAILED!\n");
    } else { 
        DEBUG_PRINT("ATTEMPT SUCCESSFUL!\n");
    }
    return hasFailed;
}

/* Checks if the given node is not NULL and that the token type matches */
ALWAYS_INLINE bool eq(ListNode* node, token_t value) {
    return (node != NULL && get_token(node)->type == value);
}

/* Returns the current head of the tail while advancing to the next token. */
ALWAYS_INLINE ListNode* next(ListNode** tailNode) {
    if (tailNode == NULL) return NULL;
    if (*tailNode == NULL) return NULL;
    ListNode* headNode = *tailNode;
    *tailNode = (*tailNode)->next;
    return headNode; 
}

/* Connects the first AST node's treeNode to a new list instance of the second AST node */
ALWAYS_INLINE void connect_ast(Ast_Node* astFirst, Ast_Node* astSecond) {
    if (astFirst == NULL) return;
    if (astSecond == NULL) return;
    ListNode* second = list_new(astSecond);
    if (astFirst->treeNode == NULL)
        astFirst->treeNode = second;
    else
        list_connect(astFirst->treeNode, list_new(astSecond));
}

/* Appends the second ast node to the back of the first ast node */
ALWAYS_INLINE void append_ast(Ast_Node* astFirst, Ast_Node* astSecond) {
    if (astFirst == NULL) return;
    if (astSecond == NULL) return;
    ListNode* second = list_new(astSecond);
    if (astFirst->treeNode == NULL)
        astFirst->treeNode = second;
    else
        list_append(astFirst->treeNode, list_new(astSecond));
}

/* Shorthand for consuming a matching token and updating headNode or pushing an error */
ALWAYS_INLINE const Token* consume_token(ListNode** headNode, ListNode** tailNode, token_t type) {
    const Token* token = get_token(*headNode);
    if (token == NULL) return NULL;
    if (eq(*headNode, type)) {
        DEBUG_PRINT("Consumed \"%s\"\n", cgpl_token_tostring[token->type]);
        *headNode = next(tailNode);
    } else {
        cgpl_error_push(token, PARSER_ERROR_BAD_TOKEN);
        return NULL;
    }
    return token;
}

/* Consumes whitespace on the tailNode until next non-whitespace token is found. Returns the new head for the tail. */
static ListNode* consume_whitespace(ListNode* headNode, ListNode** tailNode) {
    #ifdef DEBUG
    static u64 c = 0;
    #endif
    DEBUG_PARSER("consume", headNode, tailNode);
    if (headNode == NULL) return NULL;
    if (*tailNode == NULL) return headNode;
    const Token* token = get_token(headNode);
    if (IS_WHITESPACE(token->type)) {
        next(tailNode);
        #ifdef DEBUG
        c++;
        #endif
        return consume_whitespace(headNode->next, tailNode);
    }
    #ifdef DEBUG
    DEBUG_PRINT("\tRemoved %lld whitespaces...\n", c);
    c = 0;
    #endif
    return headNode;
}

PARSE_FUNC func(ListNode* headNode, ListNode** tailNode) {
    DEBUG_PARSER("parse_function", headNode, tailNode);
    return NULL;
}

/////////////////////////////////////
//          Instructions           //
/////////////////////////////////////

PARSE_FUNC expression(ListNode* headNode, ListNode** tailNode) {
    const Token* token = get_token(headNode);
    Ast_Node* astExpression = cgpl_new_ast(token, AST_EXPRESSION);
    return astExpression;
}

PARSE_FUNC assign(ListNode* headNode, ListNode** tailNode) {
    DEBUG_PARSER("assign", headNode, tailNode);
    const Token* wordToken = consume_token(&headNode, tailNode, TOKEN_WORD);
    headNode = consume_whitespace(headNode, tailNode);
    consume_token(&headNode, tailNode, TOKEN_EQUALS);
    headNode = consume_whitespace(headNode, tailNode);
    Ast_Node* astAssign = cgpl_new_ast(wordToken, AST_ASSIGN);
    Ast_Node* astExpression = expression(headNode, tailNode);
    connect_ast(astAssign, astExpression);
    return astAssign;
}

PARSE_FUNC vardecl(ListNode* headNode, ListNode** tailNode) {
    consume_token(&headNode, tailNode, TOKEN_KEYWORD_VAR);
    headNode = consume_whitespace(headNode, tailNode);
    DEBUG_PARSER("vardecl", headNode, tailNode);
    const Token* token = get_token(headNode);
    Ast_Node* astVardcl = cgpl_new_ast(token, AST_VARDECL);
    Ast_Node* astAssign = assign(headNode, tailNode);
    connect_ast(astVardcl, astAssign);
    return astVardcl;
}

PARSE_FUNC instruction(ListNode* headNode, ListNode** tailNode) {
    headNode = consume_whitespace(headNode, tailNode);
    DEBUG_PARSER("instruction", headNode, tailNode);
    Ast_Node* ast = NULL;
    if (eq(headNode, TOKEN_KEYWORD_VAR))
        ast = vardecl(headNode, tailNode);
    else if (eq(headNode, TOKEN_WORD))
        ast = assign(headNode, tailNode);
    return ast;
}

/////////////////////////////////////
//           Statements            //
/////////////////////////////////////

PARSE_FUNC statement(ListNode* headNode, ListNode** tailNode) {
    DEBUG_PARSER("statement", headNode, tailNode);
    return NULL;
}

/* Begin the file by attempting to parse either an instruction or a statement */
PARSE_FUNC file(ListNode* headNode, ListNode** tailNode) {
    DEBUG_PARSER("file", headNode, tailNode);
    const Token* sofToken = consume_token(&headNode, tailNode, TOKEN_SOF);

    Ast_Node* ast = NULL;
    attempt(statement, &ast, headNode, *tailNode, tailNode);
    if (ast != NULL) return ast;
    attempt(instruction, &ast, headNode, *tailNode, tailNode);
    if (ast != NULL) return ast;
    attempt(func, &ast, headNode, *tailNode, tailNode);
    if (ast == NULL) {
        cgpl_error_push(get_token(headNode), PARSER_PREFIX "Failed to parse file");
        return NULL;
    }
    consume_token(&headNode, tailNode, TOKEN_EOF);

    Ast_Node* fileAst = cgpl_new_ast(sofToken, AST_SOF);
    list_connect(fileAst->treeNode, list_new(ast));
    return ast;
}

Ast_Node* cgpl_parse(ListNode* tokenNode) {
    DEBUG_PRINT("Begin parsing (%lld total tokens)...\n", list_count(tokenNode));
    /* Haskell style input: (x:xs) */
    ListNode* headNode = tokenNode;
    next(&tokenNode);
    /* Entry point */
    Ast_Node* ast = file(headNode, &tokenNode);
    DEBUG_PRINT("Finished parsing!\n");
    return ast;
}

Ast_Node* cgpl_new_ast(const Token* token, ast_kind_t kind) {
    Ast_Node* node = (Ast_Node*)malloc(sizeof(Ast_Node));
    node->token = token;
    node->treeNode = NULL;
    node->kind = kind;
    return node;
}

#ifdef DEBUG
    static inline void cgpl_ast_print_internal(Ast_Node* ast, u64 c) {
        for (u64 i = 0; i < c; i++) putchar('\t');
        DEBUG_PRINT("%s: %s\n", ast_kind_tostring[ast->kind], cgpl_token_tostring[ast->token->type]);
    }

    void cgpl_ast_print(Ast_Node* ast) {
        if (ast == NULL) return;
        static u64 c = 0;
        ListNode* node = ast->treeNode;
        printf("\n");
        cgpl_ast_print_internal(ast, c);
        while (node != NULL) {
            cgpl_ast_print_internal((Ast_Node*)node->data, c++);
            cgpl_ast_print((Ast_Node*)node->data);
            node = node->next;
        }
    }
#endif