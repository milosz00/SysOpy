#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;


int fileSize(FILE *file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);

    return size;
}


void changeSentencesInFile(FILE *fileRead, FILE *fileSave, char* sentenceToChange, char* sentenceToWrite) {
    int strLen = strlen(sentenceToChange);

    int fSize = fileSize(fileRead);
    char* content = malloc((fSize+1)*sizeof(char));
    fread(content,1,fSize,fileRead);


    int rowStart,rowEnd;
    for(int i = 0;content[i];) {
        rowStart = i;
        while(content[i] && (i - rowStart) < strLen && content[i] != '\n')
            i++;
        rowEnd = i;

        int rowLength = rowEnd - rowStart + 1;
        char* row = malloc((rowLength + 1) * sizeof(char));

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];

        
        if(strcmp(sentenceToChange, row) == 0) {
            fprintf(fileSave,"%s",sentenceToWrite);
        }
        else {
            fprintf(fileSave,"%c",content[rowStart]);
            i = rowStart + 1;
        }
    }
}


int main(int argc, char** argv) {

    FILE *raport = fopen("pomiar_zad_4.txt", "w");
    if(raport == NULL){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    if(argc < 5) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    char* filenameRead = malloc((strlen(argv[1]) + 1) * sizeof(char));
    char* filenameSave = malloc((strlen(argv[2]) + 1) * sizeof(char));

    filenameRead = argv[1];
    filenameSave = argv[2];

    char* sentenceToChange = malloc((strlen(argv[3]) + 1) * sizeof(char));
    char* sentenceToWrite = malloc((strlen(argv[4]) + 1) * sizeof(char));

    sentenceToChange = argv[3];
    sentenceToWrite = argv[4];

    FILE *file1 = fopen(filenameRead, "r");
    FILE *file2 = fopen(filenameSave, "a");


    if(file1 == NULL || file2 == NULL) {
        perror("File open error");
        exit(1);
    }

    changeSentencesInFile(file1, file2, sentenceToChange, sentenceToWrite);


    fclose(file1);
    fclose(file2);


    clock_end = times(&tms_end);
    fprintf(raport, "WARIANT 1\n");
    fprintf(raport, "real time: %ld\n", clock_end - clock_start);
    fprintf(raport, "sys time: %ld\n", tms_end.tms_stime - tms_start.tms_stime);
    fprintf(raport ,"user time: %ld\n", tms_end.tms_utime - tms_start.tms_utime);

    fclose(raport);

    return 0;
}