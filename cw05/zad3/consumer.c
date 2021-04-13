#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCER 10
#define MAX_LINE_LEN 200

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Consumer arguments numbr invalid!\n");
        exit(1);
    }

    FILE *pipe = fopen(argv[1], "r");
    FILE *file = fopen(argv[2], "w+");
    const int N = atoi(argv[3]);

    char array[MAX_PRODUCER][MAX_LINE_LEN] = {0};
    char tmp[MAX_PRODUCER * MAX_LINE_LEN] = "";
    char *buffer = calloc(N + 3, sizeof(char));
    while (fread(buffer, 1, N+2, pipe) > 0) {
        strcat(tmp, buffer);
    }

    char corrector[] = "|\n";
    char* buf;
    buf = strtok(tmp, corrector);
    int max_row = 0;
    while(buf != NULL) {
      int row = atoi(buf);
      buf = strtok(NULL, corrector);
      strcat(array[row], buf);
      buf = strtok(NULL, corrector);

      if(row > max_row)
        max_row = row;
    }

    for(int i = 0; i <= max_row; i++) {
      fprintf(file, "%s\n", array[i]);
    }


    free(buffer);
    fclose(file);
    fclose(pipe);
}