#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "limits.h"
#include "game_data.h"
#include "start_state.h"

int main(int argc, char *argv[])
{
    const struct state *next_state;
    const struct state *current_state;

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