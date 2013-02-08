#define SVID_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define SIZE 8

int num[SIZE];
char *prog;

void *printhello(void *threadid)
{
	long tid;

	tid = (long) threadid;
	printf("Hello, world from thread #%ld\n", tid);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	(void) argc;

	pthread_t threads[SIZE];
	pthread_t t;

	prog = argv[0];

	for (t = 0; t < SIZE; t++) {
		if ((pthread_create(&threads[t], NULL, (void *) printhello, (void *) t)) != 0) {
			fprintf(stderr, "%s: pthread_create() error: %s\n", prog, strerror(errno));
			return EXIT_FAILURE;
		}
	}

	pthread_exit(NULL);
}
