#ifndef CGPL_ARENA_INCLUDE_H_
#define CGPL_ARENA_INCLUDE_H_

#if defined(_WIN32) && !defined(FORCE_DEFAULT_ARENA)
    #define CGPL_USE_WIN32_ARENA
#elif defined(__linux__) && !defined(FORCE_DEFAULT_ARENA)
    #define CGPL_USE_LINUX_ARENA
#else
    #define CGPL_USE_HEAP_ARENA
#endif

#if defined(CGPL_USE_WIN32_ARENA)

/* Windows oriented arena that uses virtual memory */
#include "arena_vmem_windows.h"

#elif defined(CGPL_USE_LINUX_ARENA)

/* Default arena that plainly uses the heap */
#include "arena_heap.h"

#else /* CGPL_USE_HEAP_ARENA */

/* Default arena that plainly uses the heap */
#include "arena_heap.h"

#endif /* _DEFINED(_WIN32) */

#endif /* CGPL_ARENA_INCLUDE_H_ */