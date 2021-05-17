#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>

#define ELVES_NUMBER 10
#define REINDEER_NUMBER 9

int santa_sleeping;
int send_presents;
int reindeers_ready;
int elves_help;
int elves[ELVES_NUMBER];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_ready = PTHREAD_COND_INITIALIZER;

void santa() {
    printf("Santa Claus: Hoho, here I am\n");
	while (send_presents < 3) {
        pthread_mutex_lock(&mutex);
        while(reindeers_ready < REINDEER_NUMBER && elves_help < 3) {
            puts("Mikolaj: zasypiam\n");
            santa_sleeping = 1;
            pthread_cond_wait(&santa_ready, &mutex);
        }
        santa_sleeping = 0;

		if (reindeers_ready == REINDEER_NUMBER) {
			printf("Mikolaj: dostarczam zabawki\n");
			reindeers_ready = 0;
            send_presents++;
            sleep(4);
		}
		else if (elves_help == 3)
		{
			for(int i = 0; i < ELVES_NUMBER; i++) {
                if(elves[i] == 1) {
                    printf("Mikołaj: pomagam elfowi %d\n", i);
                    elves[i] = 0;
                }
            }
            elves_help = 0;
            sleep(2);
		}
		pthread_mutex_unlock(&mutex);
	}
}

void reindeer() {
    while(true) {
        pthread_mutex_lock(&mutex);
        reindeers_ready++;
        printf("Renifer: czeka %d reniferów na Mikołaja", reindeers_ready);
        if(reindeers_ready == REINDEER_NUMBER) {
            printf("Renifer: wybudzam Mikołaja\n");
            pthread_cond_broadcast(&santa_ready);
        }
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
}

void elv() {
    while(true) {
        bool need_help = random() % 100 < 10;
        if(need_help) {
            pthread_mutex_lock(&mutex);
            elves_help++;
            printf("Elf: czeka %d elfów na Mikołaja\n", elves_help);
            if(elves_help == 3) {
                printf("Elf: wybudzam Mikołaja\n");
                pthread_cond_broadcast(&santa_ready);
            } else if(elves_help > 3) {
                printf("Elf: czeka na powrót elfów\n");
                elves_help = 3;
            }
        }
        sleep(4);
    }
}

int main() {


    pthread_t santa_thread;
    pthread_create(&santa_thread, NULL, (void* (*)(void*))santa, NULL);

    pthread_t *reindeers = calloc(REINDEER_NUMBER, sizeof(pthread_t));
    for(int i = 0; i < REINDEER_NUMBER; i++) {
        pthread_create(&reindeers[i], NULL, (void* (*)(void*))reindeer, NULL);
    }

    pthread_t *elves = calloc(ELVES_NUMBER, sizeof(pthread_t));
	for(int i = 0; i < ELVES_NUMBER; i++) {
        pthread_create(&elves[i], NULL, (void* (*)(void*))elv, NULL);
    }

    return 0;
}