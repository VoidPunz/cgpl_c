#ifndef CGPL_STRINGVIEW_H_
#define CGPL_STRINGVIEW_H_

#include <stddef.h>
#include <string.h>
#include "../types.h"

/* Compile time initialization for a StringView object */
#define STRINGVIEW(_cstr) ((StringView){.cstr = _cstr, .size = sizeof(_cstr) - 1})

typedef u32 chop_t;

typedef struct {
    const char* cstr;
    u64 size;
} StringView;

/* Creates a new string view. Intended do be used with static or otherwise constant strings whose lifetime is guaranteed. */
StringView sv_new(const char* str);
/* Chop the string view from the right indices. */
void sv_chop_right(StringView* sv, chop_t n);
/* Chop the string view from the left by n indices. */
void sv_chop_left(StringView* sv, chop_t n);
/* Split the string view at a given index. Input becomes the right-side, while the new string view object is the new left-side. */
StringView sv_split(StringView* sv, chop_t index);
/* Copies a string view object by copying cstr ptr and size */
static inline StringView sv_copy(const StringView* sv) {
    StringView nsv = sv_new(sv->cstr);
    return nsv;
}

#endif /* CGPL_STRINGVIEW_H_ */