#include <stdio.h>
#include <linux/unistd.h>

void sys_mult() {
    int mult_ret = 0;
    int x = 2, y = 5;
    int i;

    i = syscall(337, x, y, &mult_ret);
    printf("x is %d,y is %d, ret is %d \n", x, y, mult_ret);
}

void main(int argc, char *argv[]) {
    syscall(335);
    syscall(336);
    sys_mult();
}
