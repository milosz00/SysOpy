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

int fileSize(int file) {
    struct stat st;
    fstat(file,&st);
    long size = st.st_size;
    return size;
}

void changeSentencesInFile(int fileRead, int fileSave, char* sentenceToChange, char* sentenceToWrite) {
    int strLen = strlen(sentenceToChange);

    int fSize = fileSize(fileRead);
    char* content = malloc((fSize+1)*sizeof(char));
    read(fileRead,content,fSize);

    int len = strlen(sentenceToWrite);

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
            write(fileSave,sentenceToWrite,len);
        }
        else {
            char buf[2] = {content[rowStart], '\0' };
            write(fileSave,buf,2);
            i = rowStart + 1;
        }
    }
}

void writeToFile(int file, int number) {
    char buf[100];
    int bytes = sprintf(buf,"%d\n", number);
    write(file,buf,bytes);
}



int main(int argc, char** argv) {

    int raport = open("pomiar_zad_4.txt", O_WRONLY | O_APPEND);
    if(raport == -1){
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

    int file1 = open(filenameRead, O_RDONLY);
    int file2 = open(filenameSave, O_CREAT | O_WRONLY);


    if(file1 == -1 || file2 == -1) {
        perror("File open error");
        exit(1);
    }

    changeSentencesInFile(file1, file2, sentenceToChange, sentenceToWrite);


    close(file1);
    close(file2);


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