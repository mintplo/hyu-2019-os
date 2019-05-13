#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define BUFF_SIZE 1024

typedef struct {
    long data_type;
    int data_num;
    char data_buff[BUFF_SIZE];
} t_data;

int main(int argc, char *argv[])
{
    int msqid;
    int i = 0;
    t_data data;

    if ((msqid = msgget((key_t)1111, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget() is failed.\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<3; i++)
    {
        data.data_type = 1;
        data.data_num = i;
        sprintf(data.data_buff, "type=%ld, idx=%d", data.data_type, i);

        if (msgsnd(msqid, &data, sizeof(t_data) - sizeof(long), 0) == -1)
        {
            perror("msgsnd() is failed.\n");
            break;
        }
    }
 
    // msgctl(msqid, IPC_RMID, 0);
}
