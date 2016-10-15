// на убунте ускорение не наблюдается

/*
 * Я забыл про одну важную особенность ф-и clock:
 * http://stackoverflow.com/questions/2962785/c-using-clock-to-measure-time-in-multi-threaded-programs
 * 
 * Ровно поэтому вы получали ускорение равное примерно 1 и меньше (меньше, т.к. ресурсы тратятся также в ходе переключения контекстов между нитями).
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUMBERS 100000000
/*
 * FIXIT Называйте переменные в одном стиле: если назвали первую константу NUMBERS, то 
 * следующую назовите также заглавными THREADS_COUNT.
 * Откройте 1ю презентацию и посмотрите советы про стиль именования различных переменных и ф-й.
 */
#define Threads 1

/*
 * FIXIT:
 * В названии явно не хватает Task:
 */
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
/*
 * Лучше так: sum1 += Thr->a[i];
 * sum1, sum2 - не лучшие название, т.к. из них мало понятно, что именно за суммы вы вычисляете
 */
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
