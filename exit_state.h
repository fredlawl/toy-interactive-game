#ifndef _EXIT_STATE_H
#define _EXIT_STATE_H

#include "state.h"

extern const struct state exit_state;

int exit_state_init();
int exit_state_exit();
const struct state *exit_state_during();

#endif