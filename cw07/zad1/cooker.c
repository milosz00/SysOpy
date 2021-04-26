#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#include "config.h"


int main() {

    srand(getpid());
    key_t key = ftok("main", 1);

    int semaphores = semget(key, 4, 0);
    int memory = shmget(key, sizeof(memory_str), 0);

    struct sembuf lock_memory = {CAN_MODIFY_INDEX, -1, 0};
    struct sembuf decrement_space = {SPACE_INDEX, -1, 0};
    struct sembuf ops_start[2] = {lock_memory, decrement_space};

    struct sembuf unlock_memory = {CAN_MODIFY_INDEX, 1, 0};
    struct sembuf increment_created = {CREATED_INDEX, 1, 0};
    struct sembuf increment_packed = {PACKED_INDEX, 1, 0};
    struct sembuf ops_end[3] = {unlock_memory, increment_created, increment_packed};

    while(1) {

        semop(semaphores, ops_start, 2);

        int n = rand() % 10;
        printf("(%d %lu) Przygotowuje pizze %d. \n", getpid(), time(NULL), n);
        sleep(2);

        memory_str *m = shmat(memory, NULL, 0);
        
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

        int created_count = semctl(semaphores, CREATED_INDEX, GETVAL);
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

        int packed_count = semctl(semaphores, PACKED_INDEX, GETVAL);
        printf("(%d %lu) Wyjmuję pizze %d. Liczba pizz w piecu %d. Liczba pizz na stole %d\n", getpid(), time(NULL), n, created_count, packed_count + 1);

        semop(semaphores, ops_end, 3);
        shmdt(m);

        sleep(1);
    }


    return 0;
}