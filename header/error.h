#ifndef CGPL_ERROR_H_
#define CGPL_ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include "ds/stack.h"
#include "ansii.h"
#include "utils.h"
#include "types.h"

typedef struct {
    /* Token the error occured at */
    const Token* token;
    /* Accompanying string message */
    const char* msg;
} CGPLError;

/* CGPL interpreter error stack. Must be defined and initialized somewhere externally */
extern Stack g_ErrorStack;

/* Instantiates a new error */
static inline CGPLError* cgpl_error_new(const Token* token, const char* msg) {
    CGPLError* e = (CGPLError*)malloc(sizeof(CGPLError));
    if (e == NULL) ERROR_BAD_ALLOC;
    e->token = token;
    e->msg = msg;
    return e;
}

// WIP
/* Try to raise all the errors in the stack if the stack is not empty. Terminates the program if any error is raised. */
static inline void cgpl_errors_flush(void) {
    if (stack_is_empty(&g_ErrorStack)) return;
    printf(WRAP_ERROR_COL("(%d) Errors has occured:\n"), stack_count(&g_ErrorStack));
    CGPLError* e = NULL;
    while (!stack_is_empty(&g_ErrorStack)) {
        e = (CGPLError*)stack_pop(&g_ErrorStack);
        const Token* token = e->token;
        if (token == NULL)
            printf(WRAP_ERROR_COL("%s\n"), e->msg);
        else
            printf(WRAP_ERROR_COL("(%s):%u:%u: %s\n"), cgpl_token_tostring[token->type], token->line, token->col, e->msg == NULL ? "BAD MESSAGE" : e->msg);
    }
}

/* Push an error onto the error stack. Provided token cannot be null */
static inline void cgpl_error_push(const Token* token, const char* msg) {
    CGPLError* e = cgpl_error_new(token, msg);
    if (stack_push(&g_ErrorStack, (void*)e) != -1) return;
    puts(WRAP_ERROR_COL("Error stack full, raising all errors..."));
    cgpl_errors_flush();
}

/* Pop an error from the error stack, if any. Returns null if the stack is empty */
static inline CGPLError* cgpl_error_pop(void) {
    if (stack_is_empty(&g_ErrorStack)) return NULL;
    return (CGPLError*)stack_pop(&g_ErrorStack);
}

#endif /* CGPL_ERROR_H_ */