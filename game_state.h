#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "state.h"

extern const struct state game_state;

int game_state_init();
int game_state_exit();
const struct state *game_state_during();

#endif