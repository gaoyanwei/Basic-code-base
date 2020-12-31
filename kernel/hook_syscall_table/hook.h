#ifndef __SYS_HOOK_H
#define __SYS_HOOK_H

#include <linux/atomic.h>

typedef atomic_t hook_atomic_t;

typedef struct args{
    unsigned long args[7];
}hook_args_t;

#endif
