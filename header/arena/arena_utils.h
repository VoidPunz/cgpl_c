#ifndef CGPL_ARENA_UTILS_H_
#define CGPL_ARENA_UTILS_H_

#include "../types.h"
#include "../utils.h"

#define KiB(n) (n << 10)
#define MiB(n) (n << 20)
#define GiB(n) (n << 30)
#define ALIGN_UP_POW2(n, p) ((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1))

#endif /* CGPL_ARENA_UTILS_H_ */