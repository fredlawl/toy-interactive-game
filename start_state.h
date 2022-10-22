#ifndef _START_STATE_H
#define _START_STATE_H

#include "state.h"

extern const struct state start_state;

int start_state_init();
int start_state_exit();
const struct state *start_state_during();

#endif