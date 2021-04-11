#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


void child(int signum, siginfo_t *info, void *context) {

    printf("Signal number %d\n", info->si_signo); // numer sygnału
    printf("Sending PID %d\n", info->si_pid); // identyfikator pid

    // kod lub sygnał zakończenia 
    printf("Child exit code %d\n", info->si_status); // pole si_status zawiera kod zakończenia potomka lub numer sygnału, który spowodował zmiane

}


void signal_cause(int signum, siginfo_t *info, void *context) {
    printf("Signal number %d\n", info->si_signo);
    printf("Sending PID %d\n", info->si_pid);


    // si_code jest wartością określającą powód wysłania sygnału
    if(info->si_code == SI_USER) {
        printf("Signal send by user\n");
    } else if(info -> si_code == SI_KERNEL) {
        printf("Signal send by kernel\n");
    }else {
        printf("%d\n", info->si_code);
    }
}

void div_zero(int signum, siginfo_t *info, void *context) {
    printf("Signal number %d\n", info->si_signo);
    printf("Sending PID %d\n", info->si_pid);

    printf("Real user ID = %d\n",info->si_uid); // rzeczywiste id użytkownika procesu wysyłającego
    exit(0);
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }


    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    if(strcmp(argv[1], "child") == 0) {
        action.sa_sigaction = child;
        sigaction(SIGCHLD, &action, NULL);

        if(fork() == 0) {
            exit(-1);
        }


        wait(NULL);
    } else if(strcmp(argv[1], "cause") == 0) {
        action.sa_sigaction = signal_cause;
        sigaction(SIGINT, &action, NULL);

        while(1);
    } else if(strcmp(argv[1], "div") == 0) {
        action.sa_sigaction = div_zero;
        sigaction(SIGFPE, &action, NULL); // sygnał SIGFPE - wyjatek arytmetyczny

        int x = 0;
        int res = 7 / x;
        printf("%d\n", res);
    }
 
    return 0;
}