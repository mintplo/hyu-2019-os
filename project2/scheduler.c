#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_QUANTUM_BURST_TIME 5;

/**
 * 스케쥴링이 필요한 프로세스 구조체
 */
typedef struct Process
{
    int processId;
    int waitingTime;
    int burstTime;
    int priority;
} Process;

/**
 * 스케쥴러 함수 정의
 */
int getProcessCount(FILE *filePtr);

void *doFCFS(Process *processes, int processCount);
void *doSJF(Process *processes, int processCount);
void *doRR(Process *processes, int processCount);
void *doPS(Process *processes, int processCount);

void *sortProcessesWithBurstTime(Process *processes, int processCount);
void *sortProcessesWithPriority(Process *processes, int processCount);

void *printProcessesInfo(Process *processes, int processCount);
void *printGanttChart(Process *processes, int processCount);
void *printWaitingTimeInfo(Process *processes, int processCount);

int main(int argc, char *argv[])
{
    int i, processCount = 0;
    char *policy;
    Process *processes;

    char *fileString;
    FILE *filePtr;

    /**
     * 스케쥴러 실행을 위한 매개변수 오류 검증
     */
    if (argc != 3)
    {
        printf("Argument Error, Command will be %s <scheduling file relative path> <scheduleing policy in [fcfs, sjf, rr, ps]>\n", argv[0]);
        exit(1);
    }

    /**
     * 스케쥴링 정보를 담고있는 파일 열기
     */
    fileString = argv[1];
    filePtr = fopen(fileString, "r");
    if (filePtr == NULL)
    {
        printf("File Opening Error %s\n", fileString);
        exit(1);
    }

    /**
     * 스케쥴링 정책 정보 (refac needed)
     */
    policy = argv[2];
    if (!strcmp(policy, "fcfs"))
    {
        printf("Scheduling Policy: FCFS\n\n");
    }
    else if (!strcmp(policy, "sjf"))
    {
        printf("Scheduling Policy: SJF (non-preemption)\n\n");
    }
    else if (!strcmp(policy, "rr"))
    {
        printf("Scheduling Policy: Round Robin\n\n");
    }
    else if (!strcmp(policy, "ps"))
    {
        printf("Scheduling Policy: Priority Scheduling\n\n");
    }
    else
    {
        printf("Do not support %s policy\n", policy);
    }

    /**
     * 프로세스 개수 가져오기
     */
    processCount = getProcessCount(filePtr);

    /**
     * 스케쥴링을 위한 프로세스 정보 초기화 (Initialize Process Struct Array)
     * 과정
     * 
     * 1. Process 포인터 변수에 Process 구조체 사이즈 * 프로세스 개수 만큼 동적할당
     * 2. 프로세스 개수 만큼 Loop을 돌며 초기화
     * 3. 스케쥴링 정보가 담긴 파일을 PARSE and ASSIGN
     */
    processes = malloc(sizeof(Process) * processCount);
    for (i = 0; i < processCount; i++)
    {
        // 구조체 정보 초기화
        processes[i].processId = processes[i].waitingTime = processes[i].burstTime = processes[i].priority = 0;

        if (!fscanf(filePtr, "%d\t%d\t%d\n", &processes[i].processId, &processes[i].burstTime, &processes[i].priority))
        {
            printf("Didn't read process scheduling info. Please check the schedule file format");
            exit(1);
        }
    }

    /**
     * 스케쥴링 정책 실행
     */
    if (!strcmp(policy, "fcfs"))
    {
        doFCFS(processes, processCount);
    }
    else if (!strcmp(policy, "sjf"))
    {
        doSJF(processes, processCount);
    }
    else if (!strcmp(policy, "rr"))
    {
        doRR(processes, processCount);
    }
    else if (!strcmp(policy, "ps"))
    {
        doPS(processes, processCount);
    }

    /**
     * 스케쥴러 종료 전 자원 해제
     */
    fclose(filePtr);

    return 1;
}

/**
 * 목적: 프로세스의 갯수를 리턴
 * - 스케쥴링 정보가 들어있는 파일에서 처음 시작 부분의 Integer Value는 프로세스의 갯수 정보를 나타낸다.
 * - 이를 읽어서 리턴
 */
int getProcessCount(FILE *filePtr)
{
    int processCount = 0;

    if (!fscanf(filePtr, "%d\n", &processCount))
    {
        printf("Didn't read process count. Please check the schedule file format");
        exit(1);
    }

    return processCount;
}

void *doFCFS(Process *processes, int processCount)
{
    int i, totalWaitingTime = 0;

    /**
     * WAITING TIME 계산
     */
    for (i = 1; i < processCount; i++)
    {
        processes[i].waitingTime = processes[i - 1].waitingTime + processes[i - 1].burstTime;
        totalWaitingTime += processes[i].waitingTime;
    }

    /** 프로세스 정보 출력 */
    printProcessesInfo(processes, processCount);
    /** Sheduling Gantt Chart 출력 */
    printGanttChart(processes, processCount);
    /** Waiting Time 정보 출력 */
    printWaitingTimeInfo(processes, processCount);

    return NULL;
}

void *doSJF(Process *processes, int processCount)
{
    int i, totalWaitingTime = 0;

    /** Burst Time이 짧은 순서대로 정렬하도록 SORT */
    sortProcessesWithBurstTime(processes, processCount);

    /**
     * WAITING TIME 계산 with sorted Processes
     */
    for (i = 1; i < processCount; i++)
    {
        processes[i].waitingTime = processes[i - 1].waitingTime + processes[i - 1].burstTime;
        totalWaitingTime += processes[i].waitingTime;
    }

    /** 프로세스 정보 출력 */
    printProcessesInfo(processes, processCount);
    /** Sheduling Gantt Chart 출력 */
    printGanttChart(processes, processCount);
    /** Waiting Time 정보 출력 */
    printWaitingTimeInfo(processes, processCount);

    return NULL;
}

void *doRR(Process *processes, int processCount)
{
    int i, j, timeQuantum, baseTimeQuantum, totalWaitingTime = 0, totalBurstTime = 0, burst = 0;

    baseTimeQuantum = TIME_QUANTUM_BURST_TIME;

    /**
     * TOTAL BURST TIME 계산
     */
    for (i = 0; i < processCount; i++)
    {
        totalBurstTime += processes[i].burstTime;
    }

    /** 프로세스 정보 출력 */
    printProcessesInfo(processes, processCount);

    /**
     * Round Robin 만의 특별한 Gantt Chart 출력
     * @중요! (기존 출력물과 다른 포맷으로 출력합니다.)
     **/

    // 상단 BAR 출력
    timeQuantum = TIME_QUANTUM_BURST_TIME;
    for (i = 0; i < totalBurstTime; i++)
    {
        if (i % timeQuantum == 0)
        {
            if (i > 9)
                printf("   ");
            else if (i > 0)
                printf(" ");

            printf("%2d", i);
        }

        printf(" ");
    }
    printf("\n");

    // Total Burst Time이 다 소진될때 까지 실행
    while (totalBurstTime > 0)
    {
        for (i = 0; i < processCount; i++)
        {
            // 프로세스가 Burst Time을 모두 소진했으면 IGNORE
            if (processes[i].burstTime == 0)
                continue;

            /**
             * timeQuantum 할당 로직
             * 프로세스의 Burst Time이 Time Quantum 보다 작게 남아있으면 Burst Time만큼 할당
             **/
            timeQuantum = TIME_QUANTUM_BURST_TIME;
            if (processes[i].burstTime < timeQuantum)
            {
                timeQuantum = processes[i].burstTime;
            }

            /**
             * Burst Time, Total Burst Time, Waiting Time 계산
             * 
             * 각 Process의 Waiting Time 계산 (할당 받은 프로세스는 제외)
             */
            processes[i].burstTime -= timeQuantum;
            totalBurstTime -= timeQuantum;

            for (j = 0; j < processCount; j++)
            {
                if (i == j)
                    continue;

                if (processes[j].burstTime == 0)
                    continue;

                processes[j].waitingTime += timeQuantum;
            }

            /**
             * Gantt Chart PID 출력
             */
            for (j = 0; j < timeQuantum; j++)
            {
                if (burst++ % baseTimeQuantum == 0)
                    printf("");

                printf("%2d", processes[i].processId);
            }
        }
    }

    /** Waiting Time 정보 출력 */
    printWaitingTimeInfo(processes, processCount);

    return NULL;
}

void *doPS(Process *processes, int processCount)
{
    int i, totalWaitingTime = 0;

    /** Burst Time이 Priority Value가 낮은 순서대로 SORT */
    sortProcessesWithPriority(processes, processCount);

    /**
     * WAITING TIME 계산 with sorted Processes
     */
    for (i = 1; i < processCount; i++)
    {
        processes[i].waitingTime = processes[i - 1].waitingTime + processes[i - 1].burstTime;
        totalWaitingTime += processes[i].waitingTime;
    }

    /** 프로세스 정보 출력 */
    printProcessesInfo(processes, processCount);
    /** Sheduling Gantt Chart 출력 */
    printGanttChart(processes, processCount);
    /** Waiting Time 정보 출력 */
    printWaitingTimeInfo(processes, processCount);

    return NULL;
}

/**
 * for Shortest Job First(SJF)
 * Burst Time이 짧은 프로세스가 CPU를 가장 먼저 할당 받도록 순서 정렬
 */
void *sortProcessesWithBurstTime(Process *processes, int processCount)
{
    int i, j;
    Process temp;

    /** Burst Time이 짧은 프로세스가 CPU를 가장 먼저 할당 받도록 순서 정렬 */
    for (i = 0; i < processCount - 1; i++)
    {
        for (j = 0; j < processCount - 1 - i; j++)
        {
            if (processes[j].burstTime > processes[j + 1].burstTime)
            {
                temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    return NULL;
}

/**
 * for Priority Scheduling (PS)
 * Priority 값이 가장 낮은 프로세스가 CPU를 가장 먼저 할당 받도록 순서 정렬
 */
void *sortProcessesWithPriority(Process *processes, int processCount)
{
    int i, j;
    Process temp;

    /** Priority Low = Highest Priority, Priority 값이 가장 낮은 프로세스가 CPU를 가장 먼저 할당 받도록 순서 정렬 */
    for (i = 0; i < processCount - 1; i++)
    {
        for (j = 0; j < processCount - 1 - i; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    return NULL;
}

/**
 * 프로세스들의 정보를 출력
 * (CPU에게 할당 받는 순서대로 출력)
 * 
 * - Process Id
 * - Burst Time
 * - Waiting Time
 * - Priority Value
 */
void *printProcessesInfo(Process *processes, int processCount)
{
    int i, timeQunatum = TIME_QUANTUM_BURST_TIME;

    printf("Process Count: %d\n", processCount);
    printf("Time Quantum for RR: %d\n\n", timeQunatum);
    puts("+*****+*************+*************+****************+");
    puts("| PID | Burst Time | Waiting Time | Priority Value |");
    puts("+*****+************+**************+****************+");

    for (i = 0; i < processCount; i++)
    {
        printf("| %2d  |     %2d     |      %2d      |       %2d       |\n", processes[i].processId, processes[i].burstTime, processes[i].waitingTime, processes[i].priority);
        puts("+-----+------------+--------------+----------------+");
    }

    printf("\n\n");

    return NULL;
}

/**
 * CPU 스케쥴러에 의해 할당되는 Gantt Chart 정보 출력
 */
void *printGanttChart(Process *processes, int processCount)
{
    int i, j;
    int last = processes[processCount - 1].burstTime + (processCount == 1 ? 0 : processes[processCount - 1].waitingTime);

    // 상단
    printf(" ");
    for (i = 0; i < processCount; i++)
    {
        for (j = 0; j < processes[i].burstTime; j++)
            printf("--");
        printf(" ");
    }
    printf("\n|");

    // 프로세스 정보
    for (i = 0; i < processCount; i++)
    {
        for (j = 0; j < processes[i].burstTime - 1; j++)
            printf(" ");
        printf("p%d", processes[i].processId);
        for (j = 0; j < processes[i].burstTime - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");

    // 하단
    for (i = 0; i < processCount; i++)
    {
        for (j = 0; j < processes[i].burstTime; j++)
            printf("--");
        printf(" ");
    }
    printf("\n");

    // WAITING TIME 출력
    int minus = 0;
    for (i = 0; i < processCount; i++)
    {
        if (processes[i].waitingTime > 9)
            printf(" ");
        printf("%d", processes[i].waitingTime);

        if (processes[i + 1].waitingTime > 9)
        {
            minus = 1;
        }

        if (i + 1 == processCount)
            if (last > 9)
                minus = 1;

        for (j = 0; j < processes[i].burstTime - minus; j++)
            printf("  ");
    }

    if (last > 9)
        printf(" ");

    printf("%d\n", last);

    return NULL;
}

/**
 * 프로세스의 대기 시간 정보와 평균 대기 시간 출력
 */
void *printWaitingTimeInfo(Process *processes, int processCount)
{
    int i, totalWaitingTime = 0;

    printf("\n\n==== WAITING TIME INFO\n");
    for (i = 0; i < processCount; i++)
    {
        printf("Proccess [%d] PID = %d, Waiting Time = %3d \n", i, processes[i].processId, processes[i].waitingTime);
        totalWaitingTime += processes[i].waitingTime;
    }

    printf("Average Waiting Time = %.2f \n", (float)totalWaitingTime / processCount);

    return NULL;
}