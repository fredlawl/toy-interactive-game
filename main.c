#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gerror.h"
#include "limits.h"
#include "game_data.h"
#include "start_state.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    gerror_t err;
    const struct state *next_state;
    const struct state *current_state;

    err = settings_init();
    if (err) {
        fprintf(stdout, ERR_FMT, "an error occurred", err_to_msg(err)); 
        return EXIT_FAILURE;
    }

    system("clear");
    start_state.init();
    next_state = start_state.during();
    start_state.exit();

    while (next_state) {
        current_state = next_state;
        current_state->init();
        next_state = current_state->during();
        current_state->exit();
    }

    if (game_data) {
        game_data_destroy(game_data);
    }

    return EXIT_SUCCESS;
}