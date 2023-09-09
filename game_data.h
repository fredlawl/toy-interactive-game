#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include <stdint.h>
#include <time.h>

#include "gerror.h"

#define PLAYER_NAME_BUFF_LEN 64

struct player {
    char name[PLAYER_NAME_BUFF_LEN];
};

struct game_data {
    uint64_t date_created;
    uint64_t date_last_played;
    uint32_t turn;
    uint8_t num_players;
    struct player *players;
};

extern struct game_data *game_data;

gerror_t game_data_init(int no_players, struct game_data **data);
void game_data_destroy(struct game_data *data);
gerror_t game_data_save(const struct game_data *data, char *file);
gerror_t game_data_load(char *file, struct game_data **data);

#endif
