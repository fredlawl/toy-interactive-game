#ifndef _PROMPT_H
#define _PROMPT_H

#include "gerror.h"

#define PROMPT_CHAR '>'

enum tristate {
    TRIS_INVALID = 0,
    TRIS_YES,
    TRIS_NO
};

enum tristate prompt_tristate(const char *prompt);
enum tristate prompt_tristatef(const char *fmt, ...);
void prompt_str(char *dest, const size_t destlen);
void prompt_strf(char *dest, const size_t destlen, const char *fmt, ...);
gerror_t prompt_numberf(void *dest, char *buff, const size_t bufflen, const char* numfmt, const char *fmt, ...);

#endif