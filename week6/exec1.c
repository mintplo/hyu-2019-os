#include <stdio.h>
#include <unistd.h>

int main() {
	printf("before exec() call\n");
	execl("/bin/ls", "ls", "-l", (char *) 0);
	printf("after exec() call\n");

	return 0;
}
