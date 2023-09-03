#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>

#include "limits.h"
#include "game_data.h"
#include "error.h"

struct game_data *game_data = NULL;

struct __attribute__((packed)) save_file_header {
    u_int8_t signature[8];
    u_int8_t major_version;
    u_int8_t minor_version;
    u_int16_t reserved;
    u_int32_t reserved2;
};

struct __attribute__((packed)) save_file_data {
    // struct game_data 
    uint64_t date_created;
    uint64_t date_last_played;
    uint16_t turn;

    // struct players
    uint8_t num_players;
    uint8_t _r0;
    uint32_t _r1;

    uint8_t player_names_offset; // location of player names
    // fill ous rest of the 64bits 
    uint8_t _r2;
    uint16_t _r3;
    uint32_t _r4;
};

static struct save_file_header save_header = {
    .signature = {'F', 'R', 'E', 'D', 'G', 'A', 'M', 'E'},
    .major_version = 1,
    .minor_version = 0,
    .reserved = 0,
    .reserved2 = 0
};

error_t game_data_init(int num_players, struct game_data **data)
{
    time_t now = time(NULL);
    struct game_data *game_data;
    struct player *players;

    players = calloc(num_players, sizeof(*players));
    if (!players) {
        return MEM_ENOMEM;
    }

    game_data = calloc(1, sizeof(*game_data));
    if (!game_data) {
        return MEM_ENOMEM;
    }

    game_data->date_created = now;
    game_data->date_last_played = now;
    game_data->turn = 0;
    game_data->num_players = num_players;
    game_data->players = players;

    *data = game_data;

    return 0;
}

void game_data_destroy(struct game_data *data)
{
    free(data->players);
    free(data);
}

/*
 * Map game_data to save_data. This is used to simplify saving
 * and allow for game_data expansion in the future. Better to have
 * this translation two separate the two concepts.
 */
static void __game_data_to_save_data(const struct game_data *data,
                                     struct save_file_data *save_data)
{
    save_data->date_created = data->date_created;
    save_data->date_last_played = data->date_last_played;
    save_data->turn = data->turn;
    save_data->num_players = data->num_players;

    save_data->player_names_offset =
        sizeof(save_data->player_names_offset) +
        sizeof(save_data->_r2) +
        sizeof(save_data->_r3) +
        sizeof(save_data->_r4);
}

static void __save_data_to_game_data(const struct save_file_data  *save_data,
                                     struct game_data *data)
{
    data->date_created = save_data->date_created;
    data->date_last_played = save_data->date_last_played;
    data->turn = save_data->turn;
    data->num_players = save_data->num_players;
}

error_t game_data_save(const struct game_data *data, char *file)
{
    int err = 0;
    FILE *f;
    size_t bytes_written;
    struct save_file_data save_data = {0};

    __game_data_to_save_data(data, &save_data);

    f = fopen(file, "w+");
    if (!f) {
        return FILE_EOPEN;
    }

    bytes_written = fwrite(&save_header, sizeof(save_header), 1, f);
    if (bytes_written == 0 || ferror(f)) {
        err = FILE_EWRITE;
        goto out;
    }
    
    bytes_written = fwrite(&save_data, sizeof(save_data), 1, f);
    if (bytes_written == 0 || ferror(f)) {
        err = FILE_EWRITE;
        goto out;
    }

    bytes_written = fwrite(game_data->players,
                           game_data->num_players * 
                           sizeof(*game_data->players), 1, f);
    if (!bytes_written || ferror(f)) {
        err = FILE_EWRITE;
        goto out;
    }

out:
    fclose(f);
    return err;
}

static bool is_valid(const struct save_file_header *header)
{
    return  memcmp(header->signature, save_header.signature, sizeof(save_header.signature)) == 0 &&
            (header->major_version == save_header.major_version) &&
            (header->minor_version == save_header.minor_version);
}

error_t game_data_load(char *file, struct game_data **data)
{
    time_t now = time(NULL);
    int err = 0;
    FILE *f;
    size_t bytes_read;
    struct save_file_header header = {0};
    struct save_file_data save_data = {0};

    struct game_data *new_state = NULL;
    struct player *players = NULL; 

    // TODO: check for empty file

    f = fopen(file, "r");
    if (!f) {
        return FILE_EOPEN;
    }

    bytes_read = fread(&header, sizeof(header), 1, f);
    err = FILE_EREAD;
    if (bytes_read == 0 || ferror(f)) {
        goto out_err;
    }

    err = SAVE_FILE_EINVAL;
    if (!is_valid(&header)) {
        goto out_err;
    }

    bytes_read = fread(&save_data, sizeof(save_data), 1, f);
    err = FILE_EREAD;
    if (bytes_read == 0 || ferror(f)) {
        goto out_err;
    }

    new_state = calloc(1, sizeof(*new_state));
    if (!new_state) {
        err = MEM_ENOMEM;
        goto out_err;
    }

    __save_data_to_game_data(&save_data, new_state);

    players = calloc(save_data.num_players, sizeof(*players));
    if (!players) {
        err = MEM_ENOMEM;
        goto out_err;
    }


    // Need to handle the save file players_offset
    bytes_read = fread(players, sizeof(*players), save_data.num_players, f);
    err = FILE_EREAD;
    if (bytes_read == 0 || ferror(f)) {
        goto out_err;
    }

    new_state->players = players;

    fclose(f);

    if (*data) {
        game_data_destroy(*data);
    }

    new_state->date_last_played = now;
    *data = new_state;

    return 0;

out_err:
    free(players); 
    free(new_state);
    fclose(f);
    return err;
}
