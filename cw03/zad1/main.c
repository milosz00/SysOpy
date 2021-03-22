#define _POSIX_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char** argv) {

    if(argc < 2) {
        printf("Too few arguments!\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    pid_t pid;
    printf("PID procesu macierzystego: %d\n", getpid());

    for(int i = 0; i < n; i++) {

        switch (pid = fork())
        {
            case -1:
                fprintf(stderr, "Error in fork\n");
                exit(1);
                break;

            case 0:
                printf("Jestem procesem potomnym. Mój PID = %d\n", getpid());
                printf("Jestem procesem potomnym. PID Parent = %d\n", getppid());
                kill(getpid(), SIGTERM);
                break;
        }
    }

    return 0;
}