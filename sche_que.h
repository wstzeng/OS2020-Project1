#ifndef _SCHED_QUEUE_H
#define _SCHED_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#define RESERVED 10

typedef struct {
	int num;
	int front;
	int rear;
	int *data;
} Queue;

Queue *create_queue(int num);

int empty(Queue *queue);

int enqueue(Queue *queue, int data);
int dequeue(Queue *queue);

#ifdef DEBUG
#include "process.h"
void que_out(Process *proc_set, Queue *queue);
#endif

#endif