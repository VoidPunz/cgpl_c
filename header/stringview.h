#ifndef CGPL_STRINGVIEW_H_
#define CGPL_STRINGVIEW_H_

#include <stddef.h>
#include <string.h>

/* Compile time initialization of a new string view. */
#define SV_CT(str) ((String_View){.cstr = str, .size = sizeof(str) - 1})

typedef unsigned int chop_t;

typedef struct {
    const char* cstr;
    size_t size;
} String_View;

/* Creates a new string view. Intended do be used with static strings. */
String_View sv_new(const char* str);
/* Create a copy of a given string view. */
String_View sv_copy(const String_View* sv);
/* Chop the string view from the right indices. */
void sv_chop_right(String_View* sv, chop_t n);
/* Chop the string view from the left by n indices. */
void sv_chop_left(String_View* sv, chop_t n);
/* Split the string view at a given index. Input becomes the right-side, while the new string view object is the new left-side. */
String_View sv_split(String_View* sv, chop_t index);

#endif /* CGPL_STRINGVIEW_H_ */