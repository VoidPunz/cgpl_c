#ifndef CGPL_DEBUG_H_
#define CGPL_DEBUG_H_

#include <stdio.h>
#include "ansii.h"

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf(TERMINAL_COLOR_ORANGE "[D] %s:%d: " fmt TERMINAL_COLOR_DEFAULT, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif

#endif /* CGPL_DEBUG_H_ */