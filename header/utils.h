#ifndef CGPL_UTILS_H_
#define CGPL_UTILS_H_

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