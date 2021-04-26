#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#include "config.h"


int main() {

    key_t key = ftok("main", 1);

    int semaphores = semget(key, 4, 0);
    int memory = shmget(key, sizeof(memory_str), 0);

    struct sembuf lock_memory = {CAN_MODIFY_INDEX, -1, 0};
    struct sembuf decrement_created = {CREATED_INDEX, -1, 0};
    struct sembuf decrement_packed= {PACKED_INDEX, -1, 0};
    struct sembuf ops_start[3] = {lock_memory, decrement_created, decrement_packed};

    struct sembuf unlock_memory = {CAN_MODIFY_INDEX, 1, 0};
    struct sembuf increment_space = {SPACE_INDEX, 1, 0};
    struct sembuf ops_end[2] = {unlock_memory, increment_space};

    while(1) {
        semop(semaphores, ops_start, 3);

        memory_str *m = shmat(memory, NULL, 0);

        int index = m->table_index;
        while(m->pizzas_table[index].status != PACKED) {
            index = (index + 1) % MAX_PIZZA;
        }

        m->pizzas_table[index].status = SENT;
        int n = m->pizzas_table[index].value;

        int packed_count = semctl(semaphores, PACKED_INDEX, GETVAL);
        printf("(%d %lu) Pobieram pizze %d. Liczba pizz na stole %d\n", getpid(), time(NULL), n, packed_count);
        sleep(4);
        printf("(%d %lu) Dostarczam pizze %d. \n", getpid(), time(NULL), n);
        sleep(4);

        semop(semaphores, ops_end, 2);
        shmdt(m);

    }

    return 0;
}