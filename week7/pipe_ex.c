#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 32

void main(int argc, char *argv[])
{
    char put_msg[MAX_BUF];
    char get_msg[MAX_BUF];
    int pipe_desc[2];

    int child_pid;

    if (pipe(pipe_desc) == -1)
    {
        perror("Create pipe failed\n");
        exit(EXIT_FAILURE);
    }

    if ((child_pid = fork()) == 0)
    {
        printf("< child process >\n");

        while(1)
        {
            read(pipe_desc[1], get_msg, MAX_BUF);

            printf("Get Message is : %s\n", get_msg);

            if (!strncmp(get_msg, "quit", 4))
            {
                exit(EXIT_SUCCESS);
            }
            sleep(1);
        }
    }
    else if (child_pid > 0)
    {
        printf("< parent process >\n");

        while(1)
        {
            printf("Input pipe: ");

            fgets(put_msg, MAX_BUF, stdin);
            write(pipe_desc[0], put_msg, MAX_BUF);
            if (!strncmp(put_msg, "quit", 4))
            {
                exit(EXIT_SUCCESS);
            }
           // sleep(1);
        }
    }
    else
    {
        perror("Failed to create process\n");
    }
}

