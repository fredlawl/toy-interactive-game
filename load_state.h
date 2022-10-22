#ifndef _LOAD_STATE_H
#define _LOAD_STATE_H

#include "state.h"

extern const struct state load_state;

int load_state_init();
int load_state_exit();
const struct state *load_state_during();

#endif