#ifndef CGPL_STRINGVIEW_H_
#define CGPL_STRINGVIEW_H_

#include <stddef.h>
#include <string.h>

/* Compile time initialization of a new string view. */
#define SV_CT(str) ((StringView){.cstr = str, .size = sizeof(str) - 1})

typedef unsigned int chop_t;

typedef struct {
    const char* cstr;
    size_t size;
} StringView;

/* Creates a new string view. Intended do be used with static strings. */
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