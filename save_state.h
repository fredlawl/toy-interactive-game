#ifndef _SAVE_STATE_H
#define _SAVE_STATE_H

#include "state.h"

extern const struct state save_state;

int save_state_init();
int save_state_exit();
const struct state *save_state_during();

#endif