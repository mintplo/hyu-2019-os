#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int num = 0;
pthread_mutex_t mutex;

void *thread_func(void *arg);
void Error(const char *mes);

int main()
{
    pthread_t t1, t2;
    char *thread1 = "Thread_A";
    char *thread2 = "Thread_B";
    void *t_return = NULL;

    if (pthread_mutex_init(&mutex, NULL) != 0)
        Error("pthread_mutex_init Error\n");

    pthread_create(&t1, NULL, thread_func, thread1);
    pthread_create(&t2, NULL, thread_func, thread2);

    pthread_join(t1, &t_return);
    pthread_join(t2, &t_return);

    if (pthread_mutex_destroy(&mutex) != 0)
        Error("pthread_mutext_destroy Error\n");
    printf("Finale Number : %d\n", num);

    return 0;
}

void *thread_func(void *arg)
{
    int i = 0;
    for (i = 0; i<5; i++)
    {
        sleep(1);
        pthread_mutex_lock(&mutex);
        sleep(1);
        num++;
        printf("%s\t:%d\n", (char*)arg, num);
        pthread_mutex_unlock(&mutex);
    }
}

void Error(const char *mes)
{
    printf("%s\n", mes);
    exit(0);
}

