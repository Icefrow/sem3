#include <stdio.h>
#define MAX_STACK_SIZE 100

int a[9][9] = { 	
					{0, 1, 1, 1, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 1, 1, 1, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 1, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 1},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0} };
int n = 9;

struct Stack
{
   int size;
   int data[MAX_STACK_SIZE];
} stack1;

int Top(struct Stack* stack)
{
	return stack->data[stack->size];
}

void Pop(struct Stack* stack)
{
	stack->data[stack->size] = 0;
	stack->size--;
	return;
}

void Push(struct Stack* stack, int element)
{
	stack->size++;
	stack->data[stack->size] = element;
	return;
}

int isEmpty(struct Stack* stack)
{
	if(stack->size == 0)
		return 1;
	else return 0;
}

int main()
{
	int dir,i;
	Push(&stack1, 0);
	while(!isEmpty(&stack1))
	{
		dir = Top(&stack1);
		printf("%d ", dir + 1);
		Pop(&stack1);
		for(i = 8; i >= 0; i--)
			if(a[dir][i] == 1)
				Push(&stack1, i);
	}
	return 0;
}
		









