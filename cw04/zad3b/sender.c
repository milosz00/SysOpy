#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int received_signals = 0;
int is_sigusr2_signal = 0;

void sigusr1(int signum) {
    received_signals++;
}

void sigusr1_queue(int signum, siginfo_t *info, void *context) {

    printf("Number received signals: %d\n", info->si_value.sival_int);
    received_signals++;   
}

void sigusr2(int signum) {

    is_sigusr2_signal = 1;
}

int main(int argc, char** argv) {

    if(argc < 4) {
        printf("%d\n", argc);
        printf("%s\n", argv[1]);
        fprintf(stderr, "Arguments number invalidd!\n");
        exit(1);
    }


    int pid = atoi(argv[1]);
    int count = atoi(argv[2]);
    char* send_mode = malloc(sizeof(char) * (strlen(argv[3]) + 1));
    send_mode = argv[3];


    sigset_t block;
    sigfillset(&block);
    
    if(strcmp(send_mode, "SIGRT") == 0) {
        sigdelset(&block, SIGRTMIN);
        sigdelset(&block, SIGRTMIN + 1);
    }
    else {
        sigdelset(&block, SIGUSR1);
        sigdelset(&block, SIGUSR2);
    }

    sigprocmask(SIG_SETMASK, &block, NULL);


    struct sigaction action1;
    if(strcmp(send_mode, "SIGQUEUE") == 0) {
        action1.sa_sigaction = sigusr1_queue;
    }
    else {
        action1.sa_handler = sigusr1;
    }
    action1.sa_flags = SA_SIGINFO;
    sigemptyset(&action1.sa_mask);
    
    if(strcmp(send_mode, "SIGRT") == 0) {
        sigaction(SIGRTMIN, &action1, NULL);
    }
    else {
        sigaction(SIGUSR1, &action1, NULL);
    }


    struct sigaction action2;
    action2.sa_handler = sigusr2;
    action2.sa_flags = SA_SIGINFO;
    sigemptyset(&action2.sa_mask);

    if(strcmp(send_mode, "SIGRT") == 0) {
        sigaction(SIGRTMIN + 1, &action2, NULL);
    }
    else {
        sigaction(SIGUSR2, &action2, NULL);
    }


    for(int i = 0; i < count; i++) {
        if(strcmp(send_mode, "SIGQUEUE") == 0) {
            sigqueue(pid, SIGUSR1, (union sigval) {.sival_int = 0});
        }
        else {
            if(strcmp(send_mode, "SIGRT") == 0)
                kill(pid, SIGRTMIN);
            else
                kill(pid, SIGUSR1);
        }
        while(received_signals - 1 < i) {}
    }

    if(strcmp(send_mode, "SIGQUEUE") == 0) {
        sigqueue(pid, SIGUSR2, (union sigval){.sival_int = 0});
    }
    else {
        if(strcmp(send_mode, "SIGRT") == 0)
            kill(pid, SIGRTMIN + 1);
        else
            kill(pid, SIGUSR2);
    }

    while (!is_sigusr2_signal) {
    }

    printf("Sent %d signals, got back %d\n", count, received_signals);

    return 0;
}