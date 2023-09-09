#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "gerror.h"
#include "settings.h"

/*
 * out - truncated to max PATH_MAX for any buffer given
 */
error_t settings_dir(char *out)
{
    char *cfg_path = NULL;
    int bytes_written;

    cfg_path = secure_getenv(SYS_CFG_PATH_ENV);
    if (!cfg_path) {
        return ENOENT;
    }

    bytes_written = snprintf(out, PATH_MAX, "%s/%s",
            cfg_path, GAME_CFG_PATH);
    if (!bytes_written) {
        return ENOMEM;
    }

    return 0;
}

static gerror_t __create_settings_dir()
{
    int err;
    char cfg_path[PATH_MAX] = {0};

    err = settings_dir(cfg_path);
    if (err) {
        fprintf(stdout, "unable to find config path\n");
        // TODO: pick better error here
        return SAVE_FILE_EINVAL;
    }
    
    // Always attempt to create dir
    err = mkdir(cfg_path, S_IRWXU);
    if (err && errno != EEXIST) {
        fprintf(stdout, "save file path error\n");
        return SAVE_FILE_EINVAL;
    }

    return 0;
}


gerror_t settings_init()
{
    gerror_t gerr;
    gerr = __create_settings_dir();
    return gerr;
}
