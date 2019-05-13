#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void * thread_snd(void *arg);
void * thread_rcv(void *arg);

sem_t bin_sem;

char thread1[] = "A Thread(SND)";
char thread2[] = "B Thread(RCV)";
char thread3[] = "C Thread(RCV)";

int main()
{
    pthread_t t1, t2, t3;
    void *thread_result;
    int sem_value;
    int state;

    state = sem_init(&bin_sem, 0, 0);
    if (state != 0) {
        puts("sem_init() error");
        exit(1);
    }

    state = pthread_create(&t1, NULL, thread_snd, (void *)&thread1);
    state = pthread_create(&t2, NULL, thread_rcv, (void *)&thread2);
    state = pthread_create(&t3, NULL, thread_rcv, (void *)&thread3);

    pthread_join(t1, &thread_result);
    pthread_join(t2, &thread_result);
    pthread_join(t3, &thread_result);

    sem_getvalue(&bin_sem, &sem_value);
    printf("sem_getvalue: %d\n", sem_value);
    sem_destroy(&bin_sem);

    return EXIT_SUCCESS;
}

void *thread_snd(void* arg)
{
    int i = 0;
    int sema_value;

    for (i = 0; i<4; i++)
    {
        do {
            sem_getvalue(&bin_sem, &sema_value);
        }while(sema_value != 0);

        sem_getvalue(&bin_sem, &sema_value);
        printf("Execution: %s, sem_getvalue : %d \n", (char *)arg, sema_value+1);
        sem_post(&bin_sem);
    }
}

void* thread_rcv(void* arg)
{
    int i;
    int sema_value;

    for (i=0;i<2;i++)
    {
        sleep(1);
        sem_wait(&bin_sem);
        sem_getvalue(&bin_sem, &sema_value);
        printf("Execution : %s, sem_getvalue : %d \n", (char *)arg, sema_value);
    }
}

