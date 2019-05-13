#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int global_var = 0;

int main()
{
	pid_t pid, child;
	int local_var = 0;
	pid = fork();

	if (pid < 0) {
		printf("Error\n");
		return(-1);
	}
	else if (pid == 0) {
		global_var++;
		local_var++;
		printf("child pid: %d parent's pid: %d\n", getpid(), getppid());
	}
	else {
		//sleep(10);
		global_var += 5;
		local_var += 5;

		printf("parent pid : %d child pid %d\n", getpid(), pid);
	}

	printf("\n global_var : %d \n", global_var);
	printf("local_var : %d\n", local_var);

	return 0;
}
