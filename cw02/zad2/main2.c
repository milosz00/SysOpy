#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

        // if the letter is on that line, we put that line at the end of the list
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

    int file = open(filename, O_RDONLY);

    if(file == -1) {
        perror("File open error");
        exit(1);
    }

    int fSize = fileSize(file);
    char* content = malloc(fSize + 1);

    read(file,content,fSize);
    close(file);


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

void writeToFile(int file, int number) {
    char buf[100];
    int bytes = sprintf(buf,"%d\n", number);
    write(file,buf,bytes);
}


int main(int argc, char** argv) {

    int raport = open("omiar_zad_2.txt", O_WRONLY | O_APPEND);
    if(raport == -1){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    if(argc < 3) {
        fprintf(stderr,"Too few arguments!\n");
        exit(1);
    }

    char* filename = malloc((strlen(argv[2]) + 1) * sizeof(char));
    filename = argv[2];
    char letter = argv[1][0];

    printRowsWithSpecificLetter(filename, letter);
    


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