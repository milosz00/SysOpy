#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void handler_function(int signum) {
    printf("Signal received\n");
}


int main(int argc, char **argv) {

    if(argc < 3) {
        fprintf(stderr, "Arguments number invalid!\n");
        exit(1);
    }

    char* mode = malloc(sizeof(char) * (strlen(argv[1] + 1)));
    mode = argv[1];

    if(strcmp(mode, "ignore") == 0) {
        struct sigaction action;
        action.sa_flags = 0;
        sigemptyset(&action.sa_mask);
        action.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &action, NULL);
    }
    else if(strcmp(mode, "handler") == 0) {
        struct sigaction action;
        action.sa_flags = 0;
        sigemptyset(&action.sa_mask);
        action.sa_handler = handler_function;
        sigaction(SIGUSR1, &action, NULL);
    }
    else {
        sigset_t set; // contain numbers of pending signals
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);
    }

    raise(SIGUSR1);

    

    if(strcmp(mode, "mask") == 0 || strcmp(mode, "pending") == 0) {
        sigset_t set;
        sigpending(&set);
        printf("Signal SIGUSR1 belongs to signals set: %s\n", sigismember(&set, SIGUSR1) ? "true" : "false");
    }

    char* variant = malloc(sizeof(char) * strlen(argv[2] + 1));
    variant = argv[2];

    if(strcmp(variant, "fork") == 0) {
        pid_t pid = fork();

        if(pid == 0) {

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
        }

     } else if(strcmp(variant, "exec") == 0) {
        execl("./exec", "./exec", argv[1], NULL);
     }
    

    return 0;
}