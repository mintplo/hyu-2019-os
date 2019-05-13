#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler();

void main(int argc, char *argv[])
{
    struct sigaction sact;
    char c;

    printf("(1) Ignore SIGINT signal \n");
    sact.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sact, NULL);
    while((c=getchar()) != '\n');

    printf("(2) User defined Handler \n");
    sact.sa_handler = signal_handler;
    sact.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sact, NULL);
    while((c=getchar()) != '\n');

    printf("(3) default SIGINT signal Processing \n");
    sact.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sact, NULL);
    while((c=getchar()) != '\n');
}

void signal_handler()
{
    printf("received SIGINT\n");
}

