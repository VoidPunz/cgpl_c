#ifdef _WIN32

/* Windows oriented arena that uses virtual memory */
#include "arena_vmem_windows.h"

#else

/* Default arena that plainly uses the heap */
#include "arena_std.h"

#endif