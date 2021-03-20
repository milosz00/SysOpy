#include <stdio.h>
#include <stdlib.h>
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

int rowCounting(node** tail, char* content) {

    int rowStart,rowEnd,rowsCount = 0;
    for(int i = 0;content[i];) {
        rowStart = i;
        while(content[i] && content[i] != '\n')
            i++;
        rowEnd = i;

        int rowLength = rowEnd - rowStart + 1;
        char* row = malloc((rowLength + 1)*sizeof(char));

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];

        i++;


        // put at the end of the list
        *tail = malloc(sizeof(struct node)); 
        (*tail)->value = row; 
        tail = &(*tail)->next;


        rowsCount++;
    }

    return rowsCount;

}

void printTwoFiles(char* filename1, char* filename2) {

    FILE *file1;
    FILE *file2;

    file1 = fopen(filename1, "r");
    file2 = fopen(filename2, "r");

    if(file1 == NULL || file2 == NULL) {
        perror("File open error");
        exit(1);
    }

    int fileSize1 = fileSize(file1);
    int fileSize2 = fileSize(file2);

    char* content1 = malloc(fileSize1 + 1);
    char* content2 = malloc(fileSize2 + 1);

    fread(content1, 1, fileSize1, file1);
    fread(content2, 1, fileSize2, file2);
    content1[fileSize1] = 0;
    content2[fileSize2] = 0;
    fclose(file1);
    fclose(file2);

    int rowsCount = 0;
    int rows1 = 0;
    int rows2 = 0;

    node* rowList1 = NULL;
    node** tailFirst = &rowList1;

    rows1 = rowCounting(tailFirst,content1);
    rowsCount += rows1;

    node* rowList2 = NULL;
    node** tailSecond = &rowList2;

    rows2 = rowCounting(tailSecond,content2);
    rowsCount += rows2;

    free(content1);
    free(content2);
    
    struct node* iter1 = rowList1, *tmp;
    struct node* iter2 = rowList2;

    int r1 = 0, r2 = 0;
    for (int i = 0; i < rowsCount; i++) {
            if(i%2 == 0 && r1 < rows1){
                printf("%s\n", iter1->value);
                tmp = iter1;
                iter1 = iter1->next;
                free(tmp);
                r1++;
            }
            else if(r2 < rows2){
                printf("%s\n", iter2->value);
                tmp = iter2;
                iter2 = iter2->next;
                free(tmp);
                r2++;
            }
    }


    // if file1 is longer we must print other rows from this file
    while(r1 < rows1) {
        printf("%s\n", iter1->value);
        tmp = iter1;
        iter1 = iter1->next;
        free(tmp);
        r1++;
    }

    // else we print rows from file2
    while(r2 < rows2) {
        printf("%s\n", iter2->value);
        tmp = iter2;
        iter2 = iter2->next;
        free(tmp);
        r2++;
    }

}

int main(int argc, char** argv) {

    FILE *raport = fopen("pomiar_zad_1.txt", "w");
    if(raport == NULL){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);


    char* filename1 = malloc(1024 * sizeof(char));
    char* filename2 = malloc(1024 * sizeof(char));

    if(argc < 3) {
        scanf("%s", filename1);
        scanf("%s", filename2);
    }

    else {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    
    printTwoFiles(filename1, filename2);

    clock_end = times(&tms_end);
    fprintf(raport, "WARIANT 1\n");
    fprintf(raport, "real time: %ld\n", clock_end - clock_start);
    fprintf(raport, "sys time: %ld\n", tms_end.tms_stime - tms_start.tms_stime);
    fprintf(raport ,"user time: %ld\n", tms_end.tms_utime - tms_start.tms_utime);

    fclose(raport);

    return 0;
}