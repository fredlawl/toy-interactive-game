#ifndef _LIMITS_H
#define _LIMITS_H

#ifdef __APPLE__
#include <sys/syslimits.h>
#else
#include <linux/limits.h>
#endif

#define INPUT_BUFF_LEN 64

#endif