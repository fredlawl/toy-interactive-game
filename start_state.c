#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "limits.h"
#include "start_state.h"
#include "game_state.h"
#include "load_state.h"
#include "exit_state.h"
#include "prompt.h"

int start_state_init() { return 0; }
int start_state_exit() { return 0; }

const struct state *start_state_during()
{
    int err;
    int cmd = 0;
    char input_buff[INPUT_BUFF_LEN] = {0};
    const struct state *next_state;

    system("clear");
    printf("Welcome to debug simulator!\n\n");

    printf("Choose option:\n");
    printf("%d. New game\n", 1);
    printf("%d. Load\n", 2);
    printf("%d. Exit\n", 3);

    do {
        memset(input_buff, 0, INPUT_BUFF_LEN);
        err = prompt_numberf(&cmd, input_buff, INPUT_BUFF_LEN, "%d", "%s", "> ");
        if (err) {
            cmd = -1;
        }

        switch(cmd) {
            case 1:
                next_state = &game_state;
                cmd = 0;
                break;
            case 2:
                next_state = &load_state;
                cmd = 0;
                break;
            case 3:
                next_state = &exit_state;
                cmd = 0;
                break;
            default:
                printf("Invalid command...\n");
                cmd = -1;
                break;
        }

    } while(cmd);

    return next_state;
}

const struct state start_state = {
    .init = &start_state_init,
    .exit = &start_state_exit,
    .during = &start_state_during
};