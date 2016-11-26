#include <stdio.h>

#define MAX_QUEUE_SIZE 100

int a[9][9] = { 	
					{0, 1, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 1, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 1, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 1, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 1, 1, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 1, 1},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0} };
int n = 9;
struct Queue
{
    int head;
    int tail;
    int data[MAX_QUEUE_SIZE];
} my_queue;

void Push(struct Queue* queue, int element)
{
	queue->head++;
	queue->data[queue->head] = element;
	return;
}

void Pop(struct Queue* queue)
{
	queue->data[queue->tail] = 0;
	queue->tail++;
	return;
}

int Front(struct Queue* queue)
{
	return queue->data[queue->head];
}

int Back(struct Queue* queue)
{
	return queue->data[queue->tail];
}

int isEmpty(struct Queue* queue)
{
	if (queue->head - queue->tail == -1)
		return 1;
	else return 0;
}

int main()
{
	int i,dir;
	Push(&my_queue, 0);
	my_queue.tail = 1;
	while(!isEmpty(&my_queue))
	{
		dir = Back(&my_queue);
		printf("%d ", dir+1);
		Pop(&my_queue);
		for(i = 0; i < 9; i++)
    /*
     * Можно писать просто if(a[dir][i]), что
     * равносильно if(a[dir][i] != 0)
     */
			if(a[dir][i] == 1)
				Push(&my_queue,i);
	}
	return 0;
}
		







