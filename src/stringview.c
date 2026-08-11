#include "../header/ds/stringview.h"

StringView sv_new(const char* cstr) {
    return (StringView) {
        .cstr = cstr,
        .size = strlen(cstr),
    };
}

void sv_chop_right(StringView* sv, chop_t n) {
    if (sv->size - n < 0) return;
    sv->size -= n;
}

void sv_chop_left(StringView* sv, chop_t n) {
    if (sv->size >= n) return;
    sv->size -= n;
    sv->cstr += n;
}

StringView sv_split(StringView* sv, chop_t index) {
    if (sv->size < index) return *sv;
    StringView nsv = sv_copy(sv);
    return nsv;
}