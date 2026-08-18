#ifndef CGPL_TESTS_H_
#define CGPL_TESTS_H_

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../header/ansii.h"

#define FORCE_NEVER_SAVE_IN_BUFFER() \
    setvbuf(stdout, NULL, _IONBF, 0); \
    setvbuf(stderr, NULL, _IONBF, 0);

#define TEST_CONFIG(obj)
#define TEST_RESET(obj)

#define RUN_TEST(func, obj, ...) { \
    TEST_CONFIG((obj)) \
    printf(TERMINAL_COLOR_YELLOW "\tRunning \"%s\"...\n" TERMINAL_COLOR_DEFAULT, #func); \
    func((obj), ##__VA_ARGS__); \
    printf(TERMINAL_COLOR_YELLOW "\tSuccess!\n" TERMINAL_COLOR_DEFAULT); \
    TEST_RESET((obj)) \
    }

#endif