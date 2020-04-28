#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/types.h>

#define GET_TIME 333
#define PRINTK   334

#define PARENT_CPU 0
#define CHILD_CPU  1

#define NICE_WORK -10
#define NICE_IDLE 19

#define UNIT_T() { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

typedef struct {
	char name[16];
	unsigned int t_ready;
	unsigned int t_exec;
	pid_t pid;
} Process;


int proc_exec(Process proc);
int proc_work(pid_t pid);
int proc_idle(pid_t pid);
int assign_cpu(pid_t pid, int cpu);

#endif
