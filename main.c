#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"

int main()
{
	char sched_policy[64];
	int n_proc;
	int policy;
	Process *proc_set;

	scanf("%s", sched_policy);
	scanf("%d", &n_proc);

	proc_set = (Process *)malloc((n_proc)*sizeof(Process));

	for (int i = 0; i < n_proc; i++) {
		scanf("%s%d%d", 
			proc_set[i].name, &proc_set[i].t_ready, &proc_set[i].t_exec);
		proc_set[i].pid = NOT_READY;
	}

	if (strcmp(sched_policy, "FIFO") == 0) {
		policy = FIFO;
	}
	else if (strcmp(sched_policy, "RR") == 0) {
		policy = RR;
	}
	else if (strcmp(sched_policy, "SJF") == 0) {
		policy = SJF;
	}
	else if (strcmp(sched_policy, "PSJF") == 0) {
		policy = PSJF;
	}
	else {
		fprintf(stderr, "Invalid scheduling policy: %s\n", sched_policy);
		exit(0);
	}

	if (scheduler(proc_set, n_proc, policy)) {
		fprintf(stderr, "Scheduling error.\nProgram Terminated.\n");
		exit(0);
	}

	return 0;
}
