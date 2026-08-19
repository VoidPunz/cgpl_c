#ifndef CGPL_ERROR_H_
#define CGPL_ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ansii.h"

#define ERROR_BAD_ALLOC cgpl_error_fatal("Bad allocation")
#define ERROR_UNEXPECTED_NULL_PTR cgpl_error_fatal("Bad ptr")

#define WARNING_UNEXPECTED_NULL_PTR cgpl_warning("Bad ptr")

// TODO: Need support for runtime diagnostic error handling (interpreter errors w/o crashing), need a stack datastructure...

// TODO: Refactor into a macro so __FILE__ and __LINE__ macros have the correct intended values
/* Output an error message before terminating the program */
static inline void cgpl_error_fatal(const char* format, ...) {
    printf(TERMINAL_COLOR_RED "%s:%d: FATAL: ", __FILE__, __LINE__);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(TERMINAL_COLOR_DEFAULT "\n");
    exit(EXIT_FAILURE);
}

// TODO: Refactor into a macro so __FILE__ and __LINE__ macros have the correct intended values
/* Output a warning message without terminating the program */
static inline void cgpl_warning(const char* format, ...) {
    printf(TERMINAL_COLOR_GREY "%s:%d: WARNING: ", __FILE__, __LINE__);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(TERMINAL_COLOR_DEFAULT "\n");
}

#endif /* CGPL_ERROR_H_ */