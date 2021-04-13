#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    
    if (argc != 5) {
        fprintf(stderr, "Producer arguments number invalid!\n");
        exit(1);
    }

    FILE *pipe = fopen(argv[1], "w");
    FILE *file = fopen(argv[2], "r");
    char* row = malloc(strlen(argv[3] + 1)*sizeof(char));
    row = argv[3];
    const int N = atoi(argv[4]);

    char *buffer = calloc(N + 1, sizeof(char));
    while (fread(buffer, 1, N, file) > 0) {
        sleep(rand() % 2 + 1);
        fprintf(pipe, "%s|%s\n", row, buffer);
        fflush(pipe);
    }

    free(buffer);
    fclose(file);
    fclose(pipe);
}