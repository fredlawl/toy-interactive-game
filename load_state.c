#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <linux/limits.h>
#include <string.h>

#include "limits.h"
#include "prompt.h"
#include "load_state.h"
#include "game_data.h"
#include "game_state.h"

int load_state_init() { return 0; }
int load_state_exit() { return 0; }

const struct state *load_state_during()
{
    int err;
    enum tristate p;

    char input_buff[PATH_MAX] = {0};

    if (game_data) {
        p = prompt_tristate("Are you sure you want to overwrite this session?");
        if (p != TRIS_YES) {
            return &game_state;
        }
    }

    printf("Load file path:");

    do {
        memset(input_buff, 0, PATH_MAX);
        prompt_str(input_buff, PATH_MAX);

        err = game_data_load(input_buff, &game_data);
        if (err) {
            fprintf(stdout, ERR_FMT, "an error occurred", err_to_msg(err));
        }

    } while(err);

    return &game_state;
}

const struct state load_state = {
    .init = &load_state_init,
    .exit = &load_state_exit,
    .during = &load_state_during
};