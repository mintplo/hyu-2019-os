#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define BUFF_SIZE 1024

typedef struct {
    long data_type;
    int data_num;
    char data_buff[BUFF_SIZE];
} t_data;

int main(void)
{
    int msqid;
    t_data data;
    struct msqid_ds msqstat;
    int i;

    if ((msqid = msgget((key_t)1111, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget() is failed.\n");
        exit(EXIT_FAILURE);
    }

    if (msgctl(msqid, IPC_STAT, &msqstat) == -1)
    {
        perror("msgctl() is failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("The total number of the message is %ld.\n", msqstat.msg_qnum);

    for (i=0; i<msqstat.msg_qnum; i++)
    {
        if (msgrcv(msqid, &data, sizeof(t_data) - sizeof(long), 0, 0) == -1)
        {
            perror("msgrcv() is failed.\n");
            exit(EXIT_FAILURE);
        }
        printf("%d - %s\n", data.data_num, data.data_buff);
    }

    return 0;
}
