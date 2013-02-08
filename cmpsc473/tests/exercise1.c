#define SVID_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define MESSAGE_SIZE 128

typedef struct thread_data
{
	pthread_t thread_id;
	char message[MESSAGE_SIZE];
} thdata;

static void print_message(void *ptr)
{
	thdata *data;

	data = (thdata *) ptr;

	printf("Thread %ld says %s\n", data->thread_id, data->message);
	pthread_exit(0);
}

int main()
{
	pthread_t tid1, tid2;
	thdata data1, data2;

	data1.thread_id = 1;
	strncpy(data1.message, "Hello, this is thread 1!", MESSAGE_SIZE);

	data2.thread_id = 2;
	strncpy(data2.message, "Hello, this is thread 2!", MESSAGE_SIZE);

	pthread_create(&tid1, NULL, (void *) &print_message, (void *) &data1);
	pthread_create(&tid2, NULL, (void *) &print_message, (void *) &data2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return EXIT_SUCCESS;
}
