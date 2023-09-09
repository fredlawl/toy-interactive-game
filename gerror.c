#include "gerror.h"
#include <stdint.h>

static const char* ERROR_MESSAGES[] = {
    [FILE_EOPEN] = "File could not be opened",
    [FILE_EWRITE] = "Could not write to file",
    [FILE_EREAD] = "Error reading file",
    [FILE_EEMPTY] = "File is empty",
    [FILE_EBINARY] = "File is not the right binary type",

    [SAVE_FILE_EINVAL] = "Save file is in an invalid format",

    [MEM_ENOMEM] = "Could not allocate memory",

    [INPUT_EINVAL] = "Invalid input",
};


const char* err_to_msg(gerror_t err)
{
    if (err >= ERROR_COUNT || err < 1) {
        return "Invalid error";
    }

    return ERROR_MESSAGES[err];
}
