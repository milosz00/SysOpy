#define _POSIX_C_SOURCE 200809L
#include <fenv.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int signo)
{
    printf("Into handler\n");
    while(1);
}
int main()
{
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = SA_NODEFER; // sygnał, który wywował funkcje zostanie zablokowany, chyba ze uzyto flagi sa_nodefer
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
    while(1);
    return 0;
}