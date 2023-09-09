#ifndef _GERROR_H
#define _GERROR_H

#include <errno.h>
#include <error.h>
#include <err.h>

#define ERR_FMT "%s: %s\n"

enum error {
    FILE_EOPEN = 1,
    FILE_EWRITE,
    FILE_EREAD,
    FILE_EEMPTY,
    FILE_EBINARY,

    SAVE_FILE_EINVAL,

    MEM_ENOMEM,

    INPUT_EINVAL,

    // Do not use. Exists for counting purposes
    ERROR_COUNT
};

typedef enum error gerror_t;
typedef int error_t;

const char* err_to_msg(gerror_t err);

#endif
