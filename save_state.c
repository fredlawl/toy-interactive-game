#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include "limits.h"
#include "save_state.h"
#include "game_state.h"
#include "game_data.h"
#include "prompt.h"

int save_state_init() { return 0; }
int save_state_exit() { return 0; }

const struct state *save_state_during()
{
    int err;

    char input_buff[PATH_MAX] = {0};

    if (!game_data) {
        warn("game data is not set");
        return &game_state;
    }

    printf("Save file path:");

    do {
        memset(input_buff, 0, PATH_MAX);
        prompt_str(input_buff, PATH_MAX);

        // does file already exists? prompt_tristatef y/n to overwrite
        err = game_data_save(input_buff, game_data);

    } while(err);

    return &game_state;
}

const struct state save_state = {
    .init = &save_state_init,
    .exit = &save_state_exit,
    .during = &save_state_during
};