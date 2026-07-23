#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../header/parser.h"

#define CGPL_EXTENSION ".cgpl"

bool check_extension(const char* path);

int main(int argc, char* argv[]) {
    /* Check source */
    if (argc < 2) {
        printf("Source missing (either a file path or raw source)");
        return EXIT_FAILURE;
    }

    /* Initialize source */
    FILE* fp = NULL;

    {
        char* source = argv[1];
        LexerState ls;

        if (check_extension(source)) {
            fp = fopen(source, "r");
            if (fp == NULL) cgpl_error_fatal("Failed to open file...");
            cgpl_lexer_init_state_file(&ls, fp);
            printf("Source type: File source (\"%s\")\n", source);
        } else {
            cgpl_lexer_init_state_string(&ls, source, strlen(source));
            printf("Source type: Raw source\n");
        }

        /* Begin tokenization */
        #ifdef DEBUG
            printf("Beginning tokenization...\n");
        #endif
        cgpl_lexer_tokenize(&ls);
        #ifdef DEBUG
            printf("Finished tokenization! Printing tokens...\n");
            list_print(ls.head, cgpl_lexer_print_token);
        #endif
        list_free_cascade(ls.head);
    } /* Finished with lexer and source */

    /* Finish */
    if (fp != NULL) fclose(fp);
    printf("Finished!\n");
    return EXIT_SUCCESS;
}

bool check_extension(const char *path)
{
    const char* extension = strrchr(path, '.');
    if (extension == NULL) return false;
    return strcmp(extension, CGPL_EXTENSION) == 0;
}
