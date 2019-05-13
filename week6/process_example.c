#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/unistd.h>

#define NUM_THREADS 3

void *print_hello(void *thread_id) {
	int tid = (int)thread_id;
	sleep(2);
	printf("Thread [%d]'s tgid[%d] pid[%d] \n", tid, getpid(), syscall(__NR_gettid));
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int rc;
	int i;
	int status;

	for (i=0; i<NUM_THREADS;i++) {
		printf("Creating thread [%d]\n", i);
		rc = pthread_create(&threads[i], NULL, print_hello, (void *)i);
	
		if (rc) {
			printf("Error : pthread_create()'s return code is %d\n", rc);
			exit(0);
		}
	}

	for (i=0; i<NUM_THREADS; i++) {
		rc = pthread_join(threads[i], (void **)&status);
	}

	printf("Exiting with completed program.\n");

	return 0;
}
