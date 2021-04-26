#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "config.h"

sem_t* created;
sem_t* packed;
sem_t* can_modify;
sem_t* space;

void sigterm_handler() {
    sem_close(created);
    sem_close(packed);
    sem_close(can_modify);
    sem_close(space);

    shm_unlink("/memory");

    exit(0);
}


int main() {
    signal(SIGTERM, sigterm_handler);


    created = sem_open("/created", O_RDWR, 0666);
    packed = sem_open("/packed", O_RDWR, 0666);
    can_modify = sem_open("/can_modify", O_RDWR, 0666);
    space = sem_open("/space", O_RDWR, 0666);

    int memory = shm_open("/memory", O_RDWR, 0666);

    while(1) {
        sem_wait(created);
        sem_wait(can_modify);
        sem_wait(packed);

        memory_str *m = mmap(NULL, sizeof(memory_str), PROT_WRITE, MAP_SHARED, memory, 0);

        int index = m->table_index;
        while(m->pizzas_table[index].status != PACKED) {
            index = (index + 1) % MAX_PIZZA;
        }

        m->pizzas_table[index].status = SENT;
        int n = m->pizzas_table[index].value;

        // int packed_count = semctl(semaphores, PACKED_INDEX, GETVAL);
        int packed_count;
        sem_getvalue(packed, &packed_count);
        printf("(%d %lu) Pobieram pizze %d. Liczba pizz na stole %d\n", getpid(), time(NULL), n, packed_count);
        sleep(4);
        printf("(%d %lu) Dostarczam pizze %d. \n", getpid(), time(NULL), n);
        sleep(4);

        sem_post(space);
        sem_post(can_modify);
        munmap(m, sizeof(memory_str));

    }

    return 0;
}