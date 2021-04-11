#define _POSIX_C_SOURCE 200809L
#include <fenv.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>

void handler(int sig){
    printf("RECEIVED SIGNAL\n");
}

int main(int argc, char *argv[]){
    

    struct sigaction action;
    action.sa_handler = handler;
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);


    int x;
    printf("Input number: ");
    scanf("%d", &x);
    printf("Your number is: %d\n", x);

}