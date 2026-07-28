#include "../header/stringview.h"

String_View sv_new(const char* cstr) {
    return (String_View) {
        .cstr = cstr,
        .size = strlen(cstr);
    };
}

String_View sv_copy(const String_View* sv) {
    String_View nsv;
    return nsv;
}

void sv_chop_right(String_View* sv, chop_t n) {
    if (sv.size - n < 0) return;
    sv.size -= n;
}

void sv_chop_left(String_View* sv, chop_t n) {
    if (sv.size >= n) return;
    sv.size -= n;
    sv->cstr += n;
}

String_View sv_split(String_View* sv, chop_t index) {
    if (sv->size < index) return;
    String_View nsv;
    return nsv;
}