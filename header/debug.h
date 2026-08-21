#ifndef CGPL_DEBUG_H_
#define CGPL_DEBUG_H_

#include <stdio.h>
#include <inttypes.h>
#include "ansii.h"

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf(TERMINAL_COLOR_ORANGE "[D] %s:%d: " fmt TERMINAL_COLOR_DEFAULT, __FILE__, __LINE__, ##__VA_ARGS__)
    #define DEBUG_BREAK(n) for (size_t i = 0; i < n; i++) putchar('\n');
#else
    #define DEBUG_PRINT(fmt, ...)
    #define DEBUG_BREAK
#endif

#endif /* CGPL_DEBUG_H_ */