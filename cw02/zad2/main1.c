#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;

typedef struct node {
    struct node* next;
    char* value;
} node;

int fileSize(FILE *file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);

    return size;
}

int rowCounting(node** tail, char* content, char letter) {

    int rowStart,rowEnd,rowsCount = 0;
    for(int i = 0;content[i];) {
        rowStart = i;
        while(content[i] && content[i] != '\n')
            i++;
        rowEnd = i;

        int rowLength = rowEnd - rowStart + 1;
        char* row = malloc((rowLength + 1) * sizeof(char));

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];

        i++;

        if(strchr(row,letter)) {

            *tail = malloc(sizeof(struct node)); 
            (*tail)->value = row; 
            tail = &(*tail)->next;
            
            rowsCount++;
        }
    }

    return rowsCount;

}

void printRowsWithSpecificLetter(char* filename, char letter) {

    FILE *file = fopen(filename, "r");

    if(file == NULL) {
        fprintf(stderr, "Wrong filename!\n");
        exit(1);
    }

    int fSize = fileSize(file);
    char* content = malloc(fSize + 1);

    fread(content, 1, fSize, file);
    content[fSize] = 0;
    fclose(file);


    node* rowList = NULL;
    node** tail = &rowList;
    int rowsCount = rowCounting(tail,content,letter);


    node* iter = rowList, *tmp;

    for(int i = 0; i < rowsCount; i++) {
        printf("%s\n", iter->value);
        tmp = iter;
        iter = iter->next;
        free(tmp);
    }

}


int main(int argc, char** argv) {

    FILE *raport = fopen("pomiar_zad_2.txt", "w");
    if(raport == NULL){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    if(argc < 3) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    char* filename = malloc((strlen(argv[2]) + 1) * sizeof(char));
    filename = argv[2];
    char letter = argv[1][0];

    printRowsWithSpecificLetter(filename, letter);


    clock_end = times(&tms_end);
    fprintf(raport, "WARIANT 1\n");
    fprintf(raport, "real time: %ld\n", clock_end - clock_start);
    fprintf(raport, "sys time: %ld\n", tms_end.tms_stime - tms_start.tms_stime);
    fprintf(raport ,"user time: %ld\n", tms_end.tms_utime - tms_start.tms_utime);

    fclose(raport);

    return 0;
}