// на убунте ускорение не наблюдается

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define NUMBERS 100000000
#define Threads 4

struct ForThread {
	double M;
	double D;
	int *a;
};

void* my_thread(struct ForThread* Thr)
{
	int i;
	double sum1 = 0, sum2 = 0;
	for(i = 0; i < (NUMBERS / Threads); i++)
		sum1 = sum1 + Thr->a[i];
 
	Thr->M = sum1 / (NUMBERS / Threads);
	for(i = 0; i < (NUMBERS / Threads); i++)
		sum2 = sum2 + (Thr->a[i] - Thr->M) * (Thr->a[i] - Thr->M);
	
	Thr->D = sum2 / (NUMBERS / Threads);
	return NULL;
}


int main()
{
	int n, i, j, N[Threads], res[Threads];
	double M, D, sum1, sum2;
	pthread_t thread_id[Threads];
	struct ForThread ForThread[Threads];
	for(i = 0; i < Threads; i++)
	{
		ForThread[i].a = (int*) malloc(sizeof(int) * (NUMBERS / Threads));
		for(j = 0; j < (NUMBERS / Threads); j++)
			ForThread[i].a[j] = rand() % 20 - 10;
	}

	for(i = 0; i < Threads; i++)
	res[i] = pthread_create(&thread_id[i], 
                            (pthread_attr_t *)NULL, 
                            my_thread,
                            &ForThread[i]);
	clock_t tic = clock();
	for(i = 0; i < Threads; i++)
		pthread_join(thread_id[i] , (void **) NULL);

	for(i = 0; i < Threads; i++)
		sum1 = sum1 + ForThread[i].M;
	M = sum1 / Threads;
	for(i = 0; i < Threads; i++)
		sum2 = sum2 + ForThread[i].D;
	D = sum2 / Threads;

	clock_t toc = clock();
	printf("M = %.8f\n", M);
	printf("D = %.8f\n", D);
	printf("Elapsed: %.8f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return 0;

}
