#ifndef CGPL_UTILS_H_
#define CGPL_UTILS_H_

#include <stdio.h>
#include "ansii.h"

#define WRAP_ERROR_COL(msg) (TERMINAL_COLOR_RED msg TERMINAL_COLOR_DEFAULT)
#define WRAP_WARNING_COL(msg) (TERMINAL_COLOR_GREY msg TERMINAL_COLOR_DEFAULT)
#define WARP_HIGHLIGHT_COL(msg) (TERMINAL_COLOR_GREY msg TERMINAL_COLOR_DEFAULT)

#define ERROR_BAD_ALLOC cgpl_error_fatal("Bad allocation")
#define ERROR_UNEXPECTED_NULL_PTR cgpl_error_fatal("Bad ptr")
#define WARNING_UNEXPECTED_NULL_PTR cgpl_warning("Bad ptr")

/* Output an error message before terminating the program */
#define cgpl_error_fatal(fmt, ...) { \
    printf(WRAP_ERROR_COL("%s:%d: FATAL: " fmt), __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(EXIT_FAILURE); \
    }


/* Output a warning message without terminating the program */
#define cgpl_warning(fmt, ...) \
    printf(WRAP_WARNING_COL("%s:%d: WARNING: " fmt), __FILE__, __LINE__, ##__VA_ARGS__);

#define TO_VOIDPTR(value) ((void*)(intptr_t)(value))
#define FROM_VOIDPTR(ptr, type) ((type)(intptr_t)(void*)ptr)
#define TO_VOIDPTR_U(value) ((void*)(uintptr_t)(value))
#define FROM_VOIDPTR_U(ptr, type) ((type)(uintptr_t)(void*)ptr)
#define CHECK_STR(s1, s2) (strcmp(s1, s2) == 0)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define GENERATE_ENUM(enum) enum,
#define GENERATE_STRING(str) #str,
#define CGPL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* CGPL_UTILS_H_ */