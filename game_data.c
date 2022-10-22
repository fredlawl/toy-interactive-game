#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>

#include "limits.h"
#include "game_data.h"
#include "error.h"

#define PLAYER_NAME_BUFF_LEN 64

struct game_data *game_data = NULL;

struct __attribute__((packed)) save_file_header {
    u_int8_t signature[8];
    u_int8_t major_version;
    u_int8_t minor_version;
    u_int16_t reserved;
    u_int32_t reserved2;
};

static struct save_file_header save_header = {
    .signature = {'F', 'R', 'E', 'D', 'G', 'A', 'M', 'E'},
    .major_version = 1,
    .minor_version = 0,
    .reserved = 0,
    .reserved2 = 0
};

error_t game_data_init(int no_players, struct game_data **data)
{
    time_t now = time(NULL);
    struct game_data *alloc;
    struct players *players;

    players = calloc(1, sizeof(*players) + (no_players * PLAYER_NAME_BUFF_LEN));
    if (!players) {
        return MEM_ENOMEM;
    }

    players->no_players = no_players;
    players->namelen = PLAYER_NAME_BUFF_LEN;

    alloc = calloc(1, sizeof(*alloc));
    if (!alloc) {
        return MEM_ENOMEM;
    }

    alloc->date_created = now;
    alloc->date_last_played = now;
    alloc->turn = 0;
    alloc->check = 'a';
    alloc->players = players;

    *data = alloc;

    return 0;
}

void game_data_destroy(struct game_data *data)
{
    free(data->players);
    free(data);
}

error_t game_data_save(char *file, const struct game_data *data)
{
    int err = 0;
    FILE *f;
    size_t bytes_written;

    f = fopen(file, "w+");
    if (!f) {
        return FILE_EOPEN;
    }

    bytes_written = fwrite(&save_header, sizeof(save_header), 1, f);
    if (bytes_written == 0 || ferror(f)) {
        err = FILE_EWRITE;
        goto out;
    }

    bytes_written = fwrite(game_data, sizeof(*game_data) + (game_data->players->no_players * game_data->players->namelen), 1, f);
    if (bytes_written == 0 || ferror(f)) {
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
    struct save_file_header header;

    struct game_data new_game = {0};
    struct game_data *new_state = NULL;

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

    bytes_read = fread(&new_game, sizeof(new_game), 1, f);
    err = FILE_EREAD;
    if (bytes_read == 0 || ferror(f)) {
        goto out_err;
    }

    new_state = calloc(1, sizeof(new_game) + (new_game.players->no_players * new_game.players->namelen));
    if (!new_state) {
        err = MEM_ENOMEM;
        goto out_err;
    }

    memcpy(new_state, &new_game, sizeof(new_game));
    bytes_read = fread(new_state + 1, new_game.players->namelen, new_game.players->no_players, f);
    err = FILE_EREAD;
    if (bytes_read == 0 || ferror(f)) {
        goto out_err;
    }

    fclose(f);

    if (*data) {
        game_data_destroy(*data);
    }

    new_state->date_last_played = now;
    *data = new_state;

    return 0;

out_err:
    free(new_state);
    fclose(f);
    return err;
}