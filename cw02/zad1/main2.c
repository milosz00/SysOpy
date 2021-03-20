#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/stat.h>
#include <sys/times.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;

typedef struct node {
    struct node* next;
    char* value;
} node;

int fileSize(int file) {
    struct stat st;
    fstat(file,&st);
    long size = st.st_size;
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
        char* row = calloc(sizeof(char), rowLength + 1);

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];

        i++;

        *tail = calloc(sizeof(struct node), 1); 
        (*tail)->value = row; 
        tail = &(*tail)->next;
        
        rowsCount++;
    }

    return rowsCount;

}

void printTwoFiles(char* filename1, char* filename2) {

    int file1;
    int file2;

    file1 = open(filename1,O_RDONLY);
    file2 = open(filename2, O_RDONLY);

    if(file1 == -1 || file2 == -1) {
        perror("File open error");
        exit(1);
    }

    int fileSize1 = fileSize(file1);
    int fileSize2 = fileSize(file2);

    char* content1 = malloc(fileSize1 + 1);
    char* content2 = malloc(fileSize2 + 1);

    read(file1, content1, fileSize1);
    read(file2, content2, fileSize2);
    content1[fileSize1] = 0;
    content2[fileSize2] = 0;
    close(file1);
    close(file2);

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

    while(r1 < rows1) {
        printf("%s\n", iter1->value);
        tmp = iter1;
        iter1 = iter1->next;
        free(tmp);
        r1++;
    }

    while(r2 < rows2) {
        printf("%s\n", iter2->value);
        tmp = iter2;
        iter2 = iter2->next;
        free(tmp);
        r2++;
    }

}

void writeToFile(int file, int number) {
    char buf[100];
    int bytes = sprintf(buf,"%d\n", number);
    write(file,buf,bytes);
}

int main(int argc, char** argv) {

    int raport = open("pomiar_zad_1.txt", O_WRONLY | O_APPEND);
    if(raport == -1){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    char* filename1 = calloc(1024, sizeof(char));
    char* filename2 = calloc(1024, sizeof(char));

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
    write(raport, "\nWARIANT 2\n",11);
    write(raport, "real time: ", 11);
    writeToFile(raport,clock_end - clock_start);
    write(raport, "sys time: ", 10);
    writeToFile(raport,tms_end.tms_stime - tms_start.tms_stime);
    write(raport ,"user time: ", 11);
    writeToFile(raport, tms_end.tms_utime - tms_start.tms_utime);

    close(raport);

    return 0;
}