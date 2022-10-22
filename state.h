#ifndef _STATE_H
#define _STATE_H

struct state {
    int (*init)();
    int (*exit)();
    const struct state *(*during)();
};

#endif