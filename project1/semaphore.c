#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define HUNGRY 0
#define EATING 1
#define THINKING 2
#define NUM_PHIL 5
#define EXEC_TIME 600

typedef struct philosopher
{
    unsigned short numEat;
    int state;
    long wait;
} philosopher;

philosopher phil[NUM_PHIL];
sem_t chopstick[NUM_PHIL];
/** Critical Section 을 위한 Semaphore 변수 */
sem_t sema;

// 10~500 msec wait
int idlewait()
{
    int sleepTimeMS = (rand() % 491 + 10);
    usleep(sleepTimeMS * 1000);
    return sleepTimeMS;
}

//get current time(msec)
unsigned int tick()
{
    struct timeval tv;
    gettimeofday(&tv, (void *)0);
    return tv.tv_sec * (unsigned int)1000 + tv.tv_usec / 1000;
}

void initPhil(void)
{
    /**
     * 각 철학자의 변수값을 초기화 한다.
     */
    /* .................................................................. ............................ */
    unsigned short i;

    for (i = 0; i < NUM_PHIL; i++)
    {
        phil[i].numEat = 0;
        phil[i].state = THINKING;
        phil[i].wait = 0;
    }

    // Chopsticks Semaphore Initialize to 1 (처음 집는 철학자를 위해 1로 설정 sem_wait시 바로 PICK이 가능)
    for (i = 0; i < NUM_PHIL; i++)
    {
        if (sem_init(&chopstick[i], 0, 1) < 0)
        {
            perror("semaphore initialization failed");
            exit(1);
        }
    }

    sem_init(&sema, 0, 1);
    /* .................................................................. ............................ */
}

void *dining(void *arg)
{
    unsigned short i;
    unsigned short left, right;
    unsigned int start_time;
    unsigned int start_hungry, end_hungry;

    /**
     * - 식사하는 철학자의 정보 GET = arg to i
     * - 철학자 양 옆 정보 GET = LEFT, RIGHT
     */
    /* .................................................................. ............................ */
    i = *(unsigned short *)arg;
    left = (i + NUM_PHIL - 1) % NUM_PHIL;
    right = (i + 1) % NUM_PHIL;

    /**
     * EXEC_TIME을 초과하는 경우 WHILE BREAK
     */
    srand(time((void *)0));
    start_time = tick();

    while (1)
    {
        // Random Idle Wait for STATE CHANGE TO HUNGRY
        idlewait();
        // Swith STATE From THINKING to HUNGRY
        start_hungry = tick();
        phil[i].state = HUNGRY;

        // Critical Section WAITING
        sem_wait(&sema);

        /**
         * PICK UP Both Chopsticks
         * 양쪽 젓가락을 들어야 식사를 할 수 있다.
         * (N번째 철학자는 오른쪽 젓가락을 먼저 확인, 나머지 철학자는 왼쪽 젓가락을 먼저 확인)
         * 
         * sem_wait() 함수를 이용해 CHOPSTICK이 사용중이면 WAIT CONTINUE, 유휴 상태면 PICK
         */
        if ((i + 1) != NUM_PHIL)
        {
            sem_wait(&chopstick[left]);
            sem_wait(&chopstick[right]);
        }
        else
        {
            sem_wait(&chopstick[right]);
            sem_wait(&chopstick[left]);
        }

        // Switch STATE FROM HUNGRY TO EATING (HUNGRY END)
        phil[i].numEat++;
        phil[i].state = EATING;
        end_hungry = tick();

        // Random Idle Wait for STATE CHANGE TO THINKING
        idlewait();

        // Release Both Chopsticks Because OF EATING END
        sem_post(&chopstick[left]);
        sem_post(&chopstick[right]);

        // Critical Section RELEASE
        sem_post(&sema);

        // Switch STATE FROM EATING TO THINKING (EATING END)
        phil[i].wait += end_hungry - start_hungry;
        phil[i].state = THINKING;

        // EACH THREAD EXEC TIME CHECK per 1 EATING
        // 실행 시간이 EXEC TIME을 넘기면 WHILE BREAK
        if ((tick() - start_time) / 1000 > EXEC_TIME)
        {
            break;
        }
    }
    /* .................................................................. ............................ */
}

void main(void)
{
    pthread_t t[NUM_PHIL];
    unsigned short i, args[NUM_PHIL], minCount = USHRT_MAX, maxCount = 0;
    long start, end, minWait = LONG_MAX, maxWait = 0, waitAVG = 0, waitVar = 0;
    double countAVG = 0, countVar = 0;

    srand(time((void *)0));
    start = tick();
    initPhil();

    for (i = 0; i < NUM_PHIL; i++)
        args[i] = i;

    /* .................................................................. ............................ */

    // Pthread Create for number of NUM_PHIL
    // 생성할 때 해당 Thread의 dining을 하는 철학자의 정보도 같이 넘긴다.
    pthread_create(&t[0], NULL, dining, &args[0]);
    pthread_create(&t[1], NULL, dining, &args[1]);
    pthread_create(&t[2], NULL, dining, &args[2]);
    pthread_create(&t[3], NULL, dining, &args[3]);
    pthread_create(&t[4], NULL, dining, &args[4]);

    // Thread Join
    for (i = 0; i < NUM_PHIL; i++)
        pthread_join(t[i], NULL);

    // Chopsticks Semaphore Destroy
    for (i = 0; i < NUM_PHIL; i++)
    {
        if (sem_destroy(&chopstick[i]) < 0)
        {
            perror("semaphore destroy failed");
            exit(1);
        }
    }

    sem_destroy(&sema);
    /* .................................................................. ............................ */

    end = tick();
    for (i = 0; i < NUM_PHIL; i++)
    {
        printf("Philosopher %d eating count : %d\nPhilosopher %d waiting time in HUNGRY state : %ld.%ld sec\n\n", i, phil[i].numEat, i, phil[i].wait / 1000, phil[i].wait % 1000);
        countAVG += phil[i].numEat;

        if (minCount > phil[i].numEat)
            minCount = phil[i].numEat;

        if (maxCount < phil[i].numEat)
            maxCount = phil[i].numEat;

        waitAVG += phil[i].wait;

        if (minWait > phil[i].wait)
            minWait = phil[i].wait;

        if (maxWait < phil[i].wait)
            maxWait = phil[i].wait;
    }

    countAVG /= NUM_PHIL;
    waitAVG /= NUM_PHIL;

    for (i = 0; i < NUM_PHIL; i++)
    {
        countVar += (countAVG - phil[i].numEat) * (countAVG - phil[i].numEat);
        waitVar += (waitAVG - phil[i].wait) * (waitAVG - phil[i].wait);
    }

    countVar /= NUM_PHIL;
    waitVar /= NUM_PHIL;

    printf("Min count : %d\nMax count : %d\nAVG count : %.3f\nCount variance : %.3f\n\n", minCount, maxCount, countAVG, countVar);
    printf("Min wait time in HUNGRY state : %ld.%ld sec\nMax wait time in HUNGRY state: % ld.% ld sec\nAVG wait time in HUNGRY state: % ld.% ld sec\nVariance wait time in HUNGRY state: % ld.% ld sec\n\n ",
           minWait / 1000,
           minWait % 1000,
           maxWait / 1000,
           maxWait % 1000,
           waitAVG / 1000,
           waitAVG % 1000,
           waitVar / 1000000,
           (waitVar % 1000000) / 1000);
    printf("Total run time : %ld.%ld sec\n\n", (end - start) / 1000, (end - start) % 1000);
}