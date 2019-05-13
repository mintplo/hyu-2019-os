#include <stdio.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main(int argc, char *argv[])
{
    int var;
    pid_t pid;

    var = 99;
    write(1, buf, sizeof(buf) - 1);

    printf("before fork\n");

    if ((pid = fork()) == 0) {
	execl("/bin/ls", "ls", "-l", (char *) 0);
        glob++;
        var++;
        
        printf("child\t");
    } else {
        wait();
        
        printf("parent\t");
    }
    
    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    return 0;
}
