#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include "process.h"

int assign_cpu(pid_t pid, int core)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);

	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
		perror("sched_setaffinity");
		exit(1);
	}

	return 0;
}

int proc_exec(Process proc)
{
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	}

	if (pid == 0) {
		char mes2dmesg[128];
		unsigned int strt_sec, strt_nsec, fin_sec, fin_nsec;
		syscall(GET_TIME, &strt_sec, &strt_nsec);

		for (int i = 0; i < proc.t_exec; i++) {
			UNIT_T();
		}

		syscall(GET_TIME, &fin_sec, &fin_nsec);
		sprintf(mes2dmesg, "[Project1] %d %u.%09u %u.%09u\n",
		 getpid(), strt_sec, strt_nsec, fin_sec, fin_nsec);
		syscall(PRINTK, mes2dmesg);
		exit(0);
	}

	assign_cpu(pid, CHILD_CPU);

	return pid;
}

int proc_work(pid_t pid)
{
	struct sched_param param;
	param.sched_priority = 0;
#ifdef DEBUG
	fprintf(stderr, "%d work.\n", pid);
#endif

	int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
	if (ret < 0) {
		perror("sched_setscheduler");
		return -1;
	}
	int ni = setpriority(PRIO_PROCESS, pid, NICE_WORK);

	return ret;
}

int proc_idle(pid_t pid)
{
	struct sched_param param;
	param.sched_priority = 0;
#ifdef DEBUG
	fprintf(stderr, "%d work.\n", pid);
#endif

	int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
	if (ret < 0) {
		perror("sched_setscheduler");
		return -1;
	}
	int ni = setpriority(PRIO_PROCESS, pid, NICE_IDLE);

	return ret;
}
