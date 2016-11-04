/* Нити: 1 - 2.1 сек
	2 - 1.15 сек
	3 - 0.82 сек
	4 - 0.73 сек (4 ядра)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUMBERS 100000000
#define THREADS_COUNT 4

struct timespec start, finish;

struct TaskForThread {
	double M;
	double D;
	int *a;
};

void* my_thread(struct TaskForThread* Thr)
{
	int i;
	double sumM = 0, sumD = 0;
	for(i = 0; i < (NUMBERS / THREADS_COUNT); i++)
		sumM += Thr->a[i];
 
	Thr->M = sumM / (NUMBERS / THREADS_COUNT);
	for(i = 0; i < (NUMBERS / THREADS_COUNT); i++)
		sumD += (Thr->a[i] - Thr->M) * (Thr->a[i] - Thr->M);
	
	Thr->D = sumD / (NUMBERS / THREADS_COUNT);
	return NULL;
}

int main()
{
	int n, i, j, N[THREADS_COUNT], res[THREADS_COUNT];
	double M, D, sumM, sumD;
	double elapsed;
	pthread_t thread_id[THREADS_COUNT];
	struct TaskForThread TaskForThread[THREADS_COUNT];
	for(i = 0; i < THREADS_COUNT; i++)
	{
		TaskForThread[i].a = (int*) malloc(sizeof(int) * (NUMBERS / THREADS_COUNT));
		for(j = 0; j < (NUMBERS / THREADS_COUNT); j++)
			TaskForThread[i].a[j] = rand() % 20 - 10;
	}

	for(i = 0; i < THREADS_COUNT; i++)
	res[i] = pthread_create(&thread_id[i], 
                            (pthread_attr_t *)NULL, 
                            my_thread,
                            &TaskForThread[i]);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for(i = 0; i < THREADS_COUNT; i++)
		pthread_join(thread_id[i] , (void **) NULL);

	for(i = 0; i < THREADS_COUNT; i++)
		sumM += TaskForThread[i].M;
	M = sumM / THREADS_COUNT;
	for(i = 0; i < THREADS_COUNT; i++)
		sumD = sumD + TaskForThread[i].D;
	D = sumD / THREADS_COUNT;

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("M = %.8f\n", M);
	printf("D = %.8f\n", D);
	printf("Elapsed: %.8f seconds\n", elapsed);
	return 0;
}
