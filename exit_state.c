#include <stdio.h>
#include <stdlib.h>

#include "game_data.h"
#include "exit_state.h"

int exit_state_init() { return 0; }
int exit_state_exit() { return 0; }

const struct state *exit_state_during()
{
    printf("exiting...\n");
    if (game_data) {
        game_data_destroy(game_data);
    }
    exit(EXIT_SUCCESS);
    return NULL;
}

const struct state exit_state = {
    .init = &exit_state_init,
    .exit = &exit_state_exit,
    .during = &exit_state_during
};