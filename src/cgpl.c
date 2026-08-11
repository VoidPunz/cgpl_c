#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../header/parser.h"

int main(int argc, char* argv[]) {
    /* Check source */
    if (argc < 2) {
        printf("Source missing (either a file path or raw source)");
        return EXIT_FAILURE;
    }

    char* source = argv[1];

    /* Begin tokenization */
    #ifdef DEBUG
        printf("Beginning tokenization...\n");
    #endif
    ListNode* tokenHead = cgpl_lexer_tokenize(source);
    #ifdef DEBUG
        printf("Finished tokenization! Printing tokens...\n");
        list_print(tokenHead, cgpl_lexer_print_token);
    #endif
    Ast_Node* astHead = cgpl_parse(tokenHead);
    list_free(tokenHead);
    #ifdef DEBUG
        cgpl_ast_print(astHead);
    #endif
    // TODO: free ast

    /* Finish */
    printf("Finished!\n");
    return EXIT_SUCCESS;
}