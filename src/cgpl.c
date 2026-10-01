#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../header/parser.h"
#include "../header/cgplstate.h"

#define ERROR_STACK_SIZE 400

Stack g_ErrorStack;

int main(int argc, char* argv[]) {
    /* Check source */
    if (argc < 2) {
        puts("Source missing (either a file path or raw source)");
        return EXIT_FAILURE;
    }

    /* Print the used arena type */
    #if defined(CGPL_USE_WIN32_ARENA)
    DEBUG_PRINT("[Windows Arena]: Using Windows platform-specific memory arena (virtual memory).\n");
    #elif defined(CGPL_USE_LINUX_ARENA)
    DEBUG_PRINT("[Linux Arena]: Using default non-platform-specific heap memory arena.\n");
    #else /* CGPL_USE_HEAP_ARENA */
    DEBUG_PRINT("[Default Arena]: Using default non-platform-specific heap memory arena.\n");
    #endif

    char* source = argv[1];

    /* Initialize error stack */
    stack_init(&g_ErrorStack, ERROR_STACK_SIZE);

    /* Begin tokenization */
    #ifdef DEBUG
        puts("Beginning tokenization...");
    #endif /* DEBUG */

    ListNode* tokenHead = cgpl_lexer_tokenize(source);
    Ast_Node* astHead = cgpl_parse(tokenHead);
    list_free(&tokenHead);

    #ifdef DEBUG
        cgpl_ast_print(astHead);
    #endif /* DEBUG */
    CGPLState state;
    cgpl_state_init(&state);
    // TODO: free ast

    /* Finish */
    puts("Finished!");
    return EXIT_SUCCESS;
}