#define SVID_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define SIZE 1000
#define MAX_RANGE 100

int num[SIZE];
char *prog;

static void fill_array(int *a, int size)
{
	int i;
	for (i = 0; i < size; i++)
		a[i] = rand() * MAX_RANGE;
}

void print_array(int *a, int size)
{
	int i;
	for (i = 0; i < size - 1; i++)
		printf("%d ", a[i]);
	printf("%d\n", a[i]);
}

static void *find_max(void *id)
{
	int len, i;
	long index, max;

	index = (long) id;
	len = SIZE/2;
	max = num[index * len];

	for(i = index * len + 1; i < (index + 1) * len; i++) {
		if (num[i] > max) {
			max = num[i];
		}
	}
	pthread_exit((void *) max);
}

int main(int argc, char **argv)
{
	(void) argc;

	pthread_t tid0, tid1;
	long max, max0, max1;

	prog = argv[0];

	fill_array(num, SIZE);

	if ((pthread_create(&tid0, NULL, (void *) find_max, (void *) 0)) != 0) {
		fprintf(stderr, "%s: pthread_create() error: %s\n", prog, strerror(errno));
		return EXIT_FAILURE;
	}
	if ((pthread_create(&tid1, NULL, (void *) find_max, (void *) 1)) != 0) {
		fprintf(stderr, "%s: pthread_create() error: %s\n", prog, strerror(errno));
		return EXIT_FAILURE;
	}

	pthread_join(tid0, (void **) &max0);
	pthread_join(tid1, (void **) &max1);

	if (max0 > max1)
		max = max0;
	else
		max = max1;

	printf("the max value of the random array is %ld\n", max);

	return EXIT_SUCCESS;
}
