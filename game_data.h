#ifndef _GAME_DATA_H
#define _GAME_DATA_H

#include <stdint.h>
#include <time.h>

#include "error.h"

struct __attribute__((packed)) players {
    uint32_t no_players;
    uint32_t namelen;
    char players[];
};

struct __attribute__((packed)) game_data {
    uint64_t date_created;
    uint64_t date_last_played;
    uint32_t turn;
    struct  players *players;
    uint32_t check;
};

extern struct game_data *game_data;

error_t game_data_init(int no_players, struct game_data **data);
void game_data_destroy(struct game_data *data);
error_t game_data_save(char *file, const struct game_data *data);
error_t game_data_load(char *file, struct game_data **data);

#endif