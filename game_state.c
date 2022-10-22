#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "limits.h"
#include "game_data.h"
#include "game_state.h"
#include "exit_state.h"
#include "save_state.h"
#include "load_state.h"
#include "prompt.h"

enum command {
    START = 0,
    NEXT_TURN,
    LIST_PLAYERS,
    WIN,
    SAVE,
    LOAD,
    EXIT
};


int game_state_init() { return 0; }
int game_state_exit() { return 0; }

int __print_menu()
{
    int err;
    int cmd = 0;
    char input_buff[INPUT_BUFF_LEN] = {0};

    printf("Commands:\n");
    printf("%d. Next turn\n", NEXT_TURN);
    printf("%d. List players\n", LIST_PLAYERS);
    printf("%d. Win\n", WIN);
    printf("%d. Save\n", SAVE);
    printf("%d. Load\n", LOAD);
    printf("%d. Exit\n", EXIT);

    err = prompt_numberf(&cmd, input_buff, INPUT_BUFF_LEN, "%d", "%s", "> ");
    if (err) {
        cmd = -START;
    }

    printf("\n");
    return cmd;
}

void __print_turn(const struct game_data *state)
{
    int select_player = state->turn % state->players->no_players;
    printf("turn: %d\n", state->turn);
    printf("player: %s\n\n", state->players->players + select_player * state->players->namelen);
}

void __print_players(const struct game_data *state)
{
    char *player = (char *) state->players;
    printf("List of players:\n");
    for (int i = 1; i <= state->players->no_players; i++) {
        printf("%d. %s\n", i, player + ((i - 1) * state->players->namelen));
    }
    printf("\n");
}

const struct state *game_state_during()
{
    int err;
    int i;
    int num_of_players;
    char *player;

    char input_buff[INPUT_BUFF_LEN] = {0};
    int cmd = START;

    // Game data loaded before this state
    if (!game_data) {
        do {
            memset(input_buff, 0, INPUT_BUFF_LEN);
            err = prompt_numberf(&num_of_players, input_buff, INPUT_BUFF_LEN, "%d", "%s", "How many players: ");
            if (err) {
                printf("Invalid input\n");
            }
        } while (err);

        err = game_data_init(num_of_players, &game_data);
        if (err) {
            return NULL;
        }
        
        i = game_data->players->no_players;
        player = game_data->players->players;
        do {
            memset(input_buff, 0, INPUT_BUFF_LEN);
            prompt_strf(input_buff, INPUT_BUFF_LEN, "Player %d Name:", game_data->players->no_players - i + 1);
            memcpy(player + (game_data->players->no_players - i) * game_data->players->namelen, input_buff, game_data->players->namelen);
        } while (--i);
    }

    cmd = START;
    do {
        system("clear");
        __print_turn(game_data);

        // Screen draw
        switch (cmd) {
            case LIST_PLAYERS:
                __print_players(game_data);
                break;
            case NEXT_TURN:
            case WIN:
            case START:
            case EXIT:
                break;
            default:
                printf("Invalid command: %d\n\n", cmd);
                break;
        }

        cmd = __print_menu();

        // Program update mutation
        switch (cmd) {
            case NEXT_TURN:
                game_data->turn++;
                break;
            case SAVE:
                return &save_state;
            case LOAD:
                return &load_state;
            case WIN:
                printf("WON!\n");
                return &exit_state;
            case EXIT:
                return &exit_state;
                break;
        }

    } while(cmd != EXIT);

    return NULL;
}

const struct state game_state = {
    .init = &game_state_init,
    .exit = &game_state_exit,
    .during = &game_state_during
};