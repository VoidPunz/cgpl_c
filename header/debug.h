#ifndef CGPL_DEBUG_H_
#define CGPL_DEBUG_H_

#include <stdio.h>

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf("[D] %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif

#endif /* CGPL_DEBUG_H_ */