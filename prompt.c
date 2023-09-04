#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "prompt.h"

enum tristate prompt_tristate(const char *prompt) 
{
    return prompt_tristatef("%s", prompt);
}

enum tristate prompt_tristatef(const char *fmt, ...) 
{
    char input_buff[4] = {0};
    char *input;
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    printf(" [y/n]: ");

    input = fgets(input_buff, 4, stdin);
    if (*input == 'n') {
        return TRIS_NO;
    } else if (*input == 'y') {
        return TRIS_YES;
    }

    return TRIS_INVALID;
}

void prompt_str(char *dest, const size_t destlen)
{
    return prompt_strf(dest, destlen, "", "");
}

void prompt_strf(char *dest, const size_t destlen, const char *fmt, ...)
{
    char *input = NULL;
    size_t inputlen;
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    putc('\n', stdout);
    printf("%c ", PROMPT_CHAR);

    input = fgets(dest, destlen, stdin);
    inputlen = strlen(input);

    if (input && *(input + inputlen - 1) == '\n') {
        *(input + inputlen - 1) = '\0';
    }
}

error_t prompt_numberf(void *dest, char *buff, const size_t bufflen, const char* numfmt, const char *fmt, ...)
{
    char *input = NULL;
    int err;
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);

    input = fgets(buff, bufflen, stdin);
    if (!input) {
        return INPUT_EINVAL;
    }

    err = sscanf(input, numfmt, dest);
    if (err <= 0) {
        return INPUT_EINVAL;
    }

    return 0;
}
