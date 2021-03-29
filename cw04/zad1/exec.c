#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char** argv) {

    if(strcmp(argv[1], "pending")) {
        raise(SIGUSR1);
    }

    if(strcmp(argv[1], "ignore") == 0) {
        struct sigaction action;
        sigaction(SIGUSR1, NULL, &action);
        printf("Is ignored %s\n", action.sa_handler == SIG_IGN ? "true" : "false");

    } else if(strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        sigset_t set;
        sigpending(&set);
        printf("Signal SIGUSR1 belongs to signals set: %s\n", sigismember(&set, SIGUSR1) ? "true" : "false");
    }

    return 0;
}