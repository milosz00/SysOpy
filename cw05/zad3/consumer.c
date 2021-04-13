#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCER 10
#define MAX_LINE_LEN 200

int fileSize(FILE *file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);

    return size;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./consumer pipe file N\n");
        return 1;
    }

    FILE *pipe = fopen(argv[1], "r");
    FILE *file = fopen(argv[2], "w+");
    const int N = atoi(argv[3]);

    // int fSize = fileSize(pipe);
    // char* content = malloc(fSize+1);
    // fread(content, 1, fSize, pipe);
    // printf("%s\n", content);


    char array[MAX_PRODUCER][MAX_LINE_LEN] = {0};
    char tmp[2000] = "";
    char *buffer = calloc(N + 3, sizeof(char));
    while (fread(buffer, 1, N+2, pipe) > 0) {
        // fwrite(buffer, 1, N, file);
        // printf("%s", buffer);
        strcat(tmp, buffer);
        // char row[2];
        // sscanf(buffer, "%1s", row);
        // int row_= atoi(row);
        // printf("ROW = %d\n", row_);
        // char* tmp = strdup(buffer);
        // printf("%s", tmp);
        // // int row = atoi(buffer[0]);
        // // char* tmp = buffer + 2;
        // // printf("BUM = %d|%s\n", row, tmp);
        // // strcat(array[row], tmp);
        // free(tmp);
    }

    char korektor[] = "|\n";
    char* buf;
    buf = strtok(tmp, korektor);
    int max_row = 0;
    while(buf != NULL) {
      int row = atoi(buf);
      buf = strtok(NULL, korektor);
      // printf("%d|%s\n", row, buf);
      strcat(array[row], buf);
      buf = strtok(NULL, korektor);

      if(row > max_row)
        max_row = row;
    }

    for(int i = 0; i <= max_row; i++) {
      fprintf(file, "%s\n", array[i]);
    }


    free(buffer);
    // free(content);
    fclose(file);
    fclose(pipe);
}