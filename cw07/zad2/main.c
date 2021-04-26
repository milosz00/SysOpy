#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

pid_t childs[COOKER_COUNT + SUPPLIER_COUNT];

void sigint_handler() {
    for(int i = 0; i < COOKER_COUNT + SUPPLIER_COUNT; i++) {
        kill(childs[i], SIGTERM);
    }
    printf("\n\nKoniec...\n");
}

int main() {
    signal(SIGINT, sigint_handler);

    sem_t *space = sem_open("/space", O_CREAT | O_RDWR, 0666, MAX_PIZZA);
    sem_t *created = sem_open("/created", O_CREAT | O_RDWR, 0666, 0);
    sem_t *packed = sem_open("/packed", O_CREAT | O_RDWR, 0666, 0);
    sem_t *can_modify = sem_open("/can_modify", O_CREAT | O_RDWR, 0666, 1);

    int memory = shm_open("/memory", O_CREAT | O_RDWR, 0666);
    ftruncate(memory, sizeof(memory_str));

    memory_str *m = mmap(NULL, sizeof(memory_str), PROT_WRITE, MAP_SHARED, memory, 0);
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
    munmap(m, sizeof(memory_str));

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

    sem_close(space);
    sem_close(created);
    sem_close(packed);
    sem_close(can_modify);

    sem_unlink("/space");
    sem_unlink("/created");
    sem_unlink("/packed");
    sem_unlink("/can_modify");

    shm_unlink("/memory");

    return 0;
}