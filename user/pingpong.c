#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    char b = 'x';
    
    pipe(p1); pipe(p2);
    int pid = fork();

    if (pid == 0) { // Con
        read(p1[0], &b, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &b, 1);
        exit(0);
    } else { // Cha
        write(p1[1], &b, 1);
        read(p2[0], &b, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}