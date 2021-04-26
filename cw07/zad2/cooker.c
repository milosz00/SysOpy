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

sem_t* space;
sem_t* created;
sem_t* packed;
sem_t* can_modify;

void sigterm_handler() {
    puts("sigterm");
    sem_close(space);
    sem_close(created);
    sem_close(packed);
    sem_close(can_modify);

    shm_unlink("/memory");

    exit(0);
}

int main() {

    srand(getpid());
    signal(SIGTERM, sigterm_handler);

    space = sem_open("/space", O_RDWR, 0666);
    created = sem_open("/created", O_RDWR, 0666);
    packed = sem_open("/packed", O_RDWR, 0666);
    can_modify = sem_open("/can_modify", O_RDWR, 0666);

    int memory = shm_open("/memory", O_RDWR, 0666);

    while(1) {

        sem_wait(space);
        sem_wait(can_modify);

        int n = rand() % 10;
        printf("(%d %lu) Przygotowuje pizze %d. \n", getpid(), time(NULL), n);
        sleep(2);

        memory_str *m =  mmap(NULL, sizeof(memory_str), PROT_WRITE, MAP_SHARED, memory, 0);
        
        int index_furnace;
        if(m->furnace_index == -1) {
            m->furnace_index = 0;
            index_furnace = 0;
        } else {
            index_furnace = (m->furnace_index + m->furnace_size) % MAX_PIZZA;
        }

        m->pizzas_furnace[index_furnace].status = CREATED;
        m->pizzas_furnace[index_furnace].value = n;
        m->furnace_size++;

        // int created_count = semctl(semaphores, CREATED_INDEX, GETVAL);
        int created_count;
        sem_getvalue(created, &created_count);
        printf("(%d %lu) Dodałem pizze %d. Liczba pizz w piecu %d\n", getpid(), time(NULL), n, created_count + 1);
        sleep(4);

        int index_table;
        if(m->table_index == -1) {
            m->table_index = 0;
            index_table = 0;
        } else {
            index_table = (m->table_index + m->table_size) % MAX_PIZZA;
        }

        m->pizzas_table[index_table].status = PACKED;
        m->pizzas_table[index_table].value = n;
        m->table_size++;

        // int packed_count = semctl(semaphores, PACKED_INDEX, GETVAL);
        int packed_count;
        sem_getvalue(packed, &packed_count);
        printf("(%d %lu) Wyjmuję pizze %d. Liczba pizz w piecu %d. Liczba pizz na stole %d\n", getpid(), time(NULL), n, created_count, packed_count + 1);

        // semop(semaphores, ops_end, 3);
        sem_post(created);
        sem_post(packed);
        sem_post(can_modify);
        munmap(m, sizeof(memory_str));

        sleep(1);
    }


    return 0;
}