#ifndef _ERROR_H
#define _ERROR_H

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

typedef enum error error_t;

const char* err_to_msg(error_t err);

#endif