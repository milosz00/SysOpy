#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

pid_t childs[COOKER_COUNT + SUPPLIER_COUNT];
int semaphores = -1;
int memory = -1;

void sigint_handler() {
    for(int i = 0; i < COOKER_COUNT + SUPPLIER_COUNT; i++) {
        kill(childs[i], SIGTERM);
    }
    printf("\n\nKoniec...\n");
}

int main() {
    signal(SIGINT, sigint_handler);
    key_t key = ftok("main", 1);

    semaphores = semget(key, 4, IPC_CREAT | 0666);
    semctl(semaphores, SPACE_INDEX, SETVAL, MAX_PIZZA);
    semctl(semaphores, CREATED_INDEX, SETVAL, 0);
    semctl(semaphores, PACKED_INDEX, SETVAL, 0);
    semctl(semaphores, CAN_MODIFY_INDEX, SETVAL, 1);

    memory = shmget(key, sizeof(memory_str), IPC_CREAT | 0666);
    memory_str *m = shmat(memory, NULL, 0);
    m->furnace_index = -1;
    m->table_index = -1;
    m->furnace_size = 0;
    m->table_size = 0;
    for(int i = 0; i < MAX_PIZZA; i++) {
        m->pizzas_furnace[i].status = SENT;
        m->pizzas_furnace[i].value = -1;

        m->pizzas_table[i].status = SENT;
        m->pizzas_table[i].value = -1;
    }
    shmdt(m);

    int j = 0;
    for (int i = 0; i < COOKER_COUNT; i++) {
        childs[j] = fork();
        if (childs[j] == 0) {
            execlp("./cooker", "./cooker", NULL);
            return 1;
        }
        j++;
    }

    for (int i = 0; i < SUPPLIER_COUNT; i++) {
        childs[j] = fork();
        if (childs[j] == 0) {
            execlp("./supplier", "./supplier", NULL);
            perror("test");
            return 1;
        }
        j++;
    }

    for (int i = 0; i < COOKER_COUNT + SUPPLIER_COUNT; i++) {
        wait(0);
    }

    if (semaphores != -1) {
        semctl(semaphores, 0, IPC_RMID);
    }
    if (memory != -1) {
        shmctl(memory, IPC_RMID, NULL);
    }

    return 0;
}