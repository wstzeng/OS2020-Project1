#include "sche_que.h"

#ifdef DEBUG
#include "process.h"
void que_out(Process *proc_set, Queue *queue)
{
	int tmp = queue->front;
	fprintf(stderr, "Queue(%d, %d)/%d: {", queue->front, queue->rear, queue->num);
	while (tmp != queue->rear) {
		fprintf(stderr, "%d-%s ", tmp, proc_set[queue->data[tmp]].name);
		tmp = ((tmp + 1) % queue->num);
	}
	fprintf(stderr, "}\n");
}
#endif

Queue *create_queue(int num)
{
	Queue *new_queue = (Queue *)malloc(sizeof(Queue));
	new_queue->num = num + RESERVED;
	new_queue->front = 0;
	new_queue->rear = 0;
	new_queue->data = (int *)malloc(new_queue->num*sizeof(int));
	return new_queue;
}

int empty(Queue *queue)
{
	return queue->front == queue->rear;
}

int enqueue(Queue *queue, int data)
{
	queue->data[queue->rear] = data;
	queue->rear = ((queue->rear + 1) % queue->num);
	return 0;
}

int dequeue(Queue *queue)
{
	int ret = -1;
	if (!empty(queue)) {
		ret = queue->data[queue->front];
		queue->front = ((queue->front + 1) % queue->num);
	}
	return ret;
}
