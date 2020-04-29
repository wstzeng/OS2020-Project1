#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "scheduler.h"
#include "process.h"
#include "sche_que.h"

#define TIME_QUAT 500

/* Index of current running process */
static int curr;
/* Number of finished process */
static int n_proc_fin;
/* Current unit time clock */
static int ut_clk;
/* RR */
static int t_curr;

int cmp(const void *proc_a, const void *proc_b)
{
	return ((Process *)proc_a)->t_ready-((Process *)proc_b)->t_ready;
}

int next_proc_FIFO(Process *proc_set, int n_proc)
{
	if (curr != NOT_READY)
		return curr;

	int ret = NOT_READY;
	for (int i = 0; i < n_proc; i++) {
		if (proc_set[i].pid == NOT_READY || proc_set[i].t_exec == 0)
			continue;
		if (ret == NOT_READY)
			ret = i;
	}
	return ret;
}

int next_proc_RR(Process *proc_set, int n_proc, Queue *rr_que)
{
	if (curr != NOT_READY && ((ut_clk - t_curr) % TIME_QUAT) != 0)
		return curr;

	int ret = NOT_READY;
	if (curr == NOT_READY && !empty(rr_que)) {
#ifdef DEBUG
		que_out(proc_set, rr_que);
		fprintf(stderr, "RR Cond-1: %d\n", ret);
#endif
		ret = dequeue(rr_que);
		t_curr = ut_clk;
	}
	else if (curr != NOT_READY && ((ut_clk - t_curr) % TIME_QUAT) == 0) {
		if (proc_set[curr].t_exec != 0)
			enqueue(rr_que, curr);
		ret = dequeue(rr_que);
		t_curr = ut_clk;
#ifdef DEBUG
		fprintf(stderr, "RR Cond-2: %d\n", ret);
#endif
	}

	return ret;
}

int next_proc_SJF(Process *proc_set, int n_proc)
{
	if (curr != NOT_READY)
		return curr;

	int ret = NOT_READY;
	for (int i = 0; i < n_proc; i++) {
		if (proc_set[i].pid == NOT_READY || proc_set[i].t_exec == 0)
			continue;
		if (ret == NOT_READY || proc_set[i].t_exec < proc_set[ret].t_exec)
			ret = i;
	}

	return ret;
}

int next_proc_PSJF(Process *proc_set, int n_proc)
{
	int ret = NOT_READY;
	for (int i = 0; i < n_proc; i++) {
		if (proc_set[i].pid == NOT_READY || proc_set[i].t_exec == 0)
			continue;
		if (ret == NOT_READY || proc_set[i].t_exec < proc_set[ret].t_exec)
			ret = i;
	}

	return ret;
}

int scheduler(Process *proc_set, int n_proc, int policy)
{
	qsort(proc_set, n_proc, sizeof(Process), cmp);

	curr = NOT_READY;
	n_proc_fin = 0;
	ut_clk = 0;
	/* queue used in RR */
	Queue *rr_que = create_queue(n_proc);
	assign_cpu(getpid(), PARENT_CPU);
	proc_work(getpid());

#ifdef DEBUG
	int newline = 1;
#endif

	while (1) {
#ifdef DEBUG
		/*
		for (int i = 0; i < 80; i++)
			fprintf(stderr, "=");
		*/
		if (!newline)
			fprintf(stderr, "\033[A");
		else if (policy == RR)
			que_out(proc_set, rr_que);
		fprintf(stderr, "- T = %d -\n", ut_clk);
		newline = 0;
#endif
		if (curr != NOT_READY && proc_set[curr].t_exec == 0) {
			waitpid(proc_set[curr].pid, NULL, 0);
			printf("%s %d\n", proc_set[curr].name, proc_set[curr].pid);
			fflush(stdout);
			// char to_stdout[128];
			// sprintf(to_stdout, "%s %d\n", proc_set[curr].name, proc_set[curr].pid);
			// write(STDOUT_FILENO, to_stdout, strlen(to_stdout));
#ifdef DEBUG
			fprintf(stderr, "%s [%d] end at time %d.\n", 
				proc_set[curr].name, proc_set[curr].pid, ut_clk);
			newline = 1;
#endif
			curr = -1;
			n_proc_fin++;

			if (n_proc_fin == n_proc)
				break;
		}

		for (int i = 0; i < n_proc; i++) {
			if (proc_set[i].t_ready == ut_clk) {
				proc_set[i].pid = proc_exec(proc_set[i]);
				proc_idle(proc_set[i].pid);
				if (policy == RR)
					enqueue(rr_que, i);
#ifdef DEBUG
				fprintf(stderr, "%s [%d] ready at time %d.\n", 
					proc_set[i].name, proc_set[i].pid, ut_clk);
				newline = 1;
#endif
			}
		}

		int next = NOT_READY;
		switch (policy) {
		case (FIFO):
			next = next_proc_FIFO(proc_set, n_proc);
			break;
		case (RR):
			next = next_proc_RR(proc_set, n_proc, rr_que);
			break;
		case (SJF):
			next = next_proc_SJF(proc_set, n_proc);
			break;
		case (PSJF):
			next = next_proc_PSJF(proc_set, n_proc);
			break;
		default:
			break;
		}

		if (next != NOT_READY) {
			if (curr != next) {
#ifdef DEBUG
				fprintf(stderr, "Context Switch [%s > %s]\n",
					curr != NOT_READY ? proc_set[curr].name : "NONE", 
					next != NOT_READY ? proc_set[next].name : "NONE");
				newline = 1;
#endif
				proc_work(proc_set[next].pid);
				if (curr != NOT_READY)
					proc_idle(proc_set[curr].pid);
				curr = next;
			}
		}

		UNIT_T();
		if (curr != NOT_READY)
			proc_set[curr].t_exec--;
		ut_clk++;
	}

	return 0;
}
