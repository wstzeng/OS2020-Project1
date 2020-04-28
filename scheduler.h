#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "process.h"

#define FIFO 0
#define RR   1
#define SJF  2
#define PSJF 3
#define NOT_READY -1

int scheduler(Process *proc_set, int n_proc, int policy);

#endif
