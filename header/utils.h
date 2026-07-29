#ifndef CGPL_UTILS_H_
#define CGPL_UTILS_H_

#define GENERATE_ENUM(enum) enum,
#define GENERATE_STRING(str) #str,
#define CGPL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* CGPL_UTILS_H_ */