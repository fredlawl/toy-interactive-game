#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "gerror.h"

// TODO: this could be different depending on OS
#define SYS_CFG_PATH_ENV "XDG_CONFIG_HOME"

#define GAME_CFG_PATH "fredgame"
#define SAVE_FILE "save.fred"
#define SAVE_SLOTS 3

gerror_t settings_init();
error_t settings_dir(char *out);
#endif
