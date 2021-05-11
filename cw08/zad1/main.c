#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include <math.h>
#include <sys/times.h>

#define MAX_VALUE 256
#define MAX_LINE_LENGTH 128
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define WHITESPACE " \t\r\n"

int** image;
int** new_image;
int width;
int height;

int threads_count;
int* intervals;

void free_memory() {
    for(int i = 0; i < height; i++) 
        free(image[i]);
    free(image);

    for(int i = 0; i < height; i++) 
        free(new_image[i]);
    free(new_image);
}

int calculate_time(struct timespec *start_time) {
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    int time = (end_time.tv_sec - start_time->tv_sec) * 1000000;
    time += (end_time.tv_nsec - start_time->tv_nsec) / 1000.0;
    return time;
}

void set_properties(char *buffer)
{
    char *w;
    char *h;

    w = strtok(buffer, WHITESPACE);
    h = strtok(NULL, WHITESPACE);
    width = atoi(w);
    height = atoi(h);
}
void load_row(char *line, int r)
{
    char *i;
    char *rest = line;
    for (int col = 0; col < width; col++)
    {
        i = strtok_r(rest, WHITESPACE, &rest);
        image[r][col] = atoi(i);
    }
}
void load_image(char *input_file)
{
    FILE *file = fopen(input_file, "r");

    char buffer[MAX_LINE_LENGTH + 1];
    
    // skip header
    fgets(buffer, MAX_LINE_LENGTH, file);
    
    // set width and height
    fgets(buffer, MAX_LINE_LENGTH, file);
    set_properties(buffer);

    // skip M
    fgets(buffer, MAX_LINE_LENGTH, file);

    image = calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++)
        image[i] = calloc(width, sizeof(int));

    char *line = NULL;
    size_t len = 0;
    for (int r = 0; r < height; r++)
    {
        getline(&line, &len, file);
        load_row(line, r);
    }
    fclose(file);
}

void write_image(char* output_file) {
    FILE* file = fopen(output_file, "w");
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", 256);

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            fprintf(file, "%d ", new_image[y][x]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int numbers_handler(int *thread_index) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int k = *thread_index;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if(image[y][x] < intervals[k]) {
                new_image[y][x] = 255 - image[y][x];
            }
        }
    }

    return calculate_time(&start_time);
}

int block_handler(int *thread_index) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int k = *thread_index;
    int x_min = k * ceil((double) width / threads_count);
    int x_max = min((k + 1) * ceil((double) width / threads_count) - 1, width - 1);

    for(int y = 0; y < height; y++) {
        for(int x = x_min; x <= x_max; x++) {
            new_image[y][x]  = 255 - image[y][x];
        }
    }

    return calculate_time(&start_time);
}

int main(int argc, char** argv) {

    if(argc != 5) {
        perror("Usage: ./main threads_count mode input_file output_file\n");
        exit(1);
    }

    threads_count = atoi(argv[1]);
    char* mode = argv[2];
    char* input_file = argv[3];
    char* output_file = argv[4];

    printf("------   MODE: %s   ------\n\n", mode);

    load_image(input_file);
    
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); 

    new_image = calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++) {
        new_image[i] = calloc(width, sizeof(int));
    }

    pthread_t *threads = calloc(threads_count, sizeof(pthread_t));
    int *args = calloc(threads_count, sizeof(int));

    intervals = calloc(threads_count, sizeof(int));
    for(int i = 0; i < threads_count; i++) {
        intervals[i] = MAX_VALUE / threads_count + i * (MAX_VALUE / threads_count); 
    }


    for(int i = 0; i < threads_count; i++) {
        int (*start_routine)(int *);
        if(strcmp(mode, "numbers") == 0) {
            start_routine = numbers_handler;
        } 

        if(strcmp(mode, "block") == 0) {
            start_routine = block_handler;
        }

        args[i] = i;
        pthread_create(&threads[i], NULL, (void*(*)(void *))start_routine, args + i);
    }

    for(int i = 0; i < threads_count; i++) {
        int time;
        pthread_join(threads[i], (void *)&time);
        printf("\tTHREAD NO. %d lasted %d microseconds\n", i, time);
    }

    printf("\n\tTOTAL TIME: %d microseconds\n\n\n", calculate_time(&start_time));

    write_image(output_file);

    free(threads);
    free(args);
    free(intervals);
    free_memory();

    return 0;
}