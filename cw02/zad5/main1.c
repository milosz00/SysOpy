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


void writeFiftyLetterSentence(FILE* file, char* content, int fSize) {

    int rowStart, rowEnd;
    for(int i = 0; content[i]; ) {
        rowStart = i;
        while(content[i]  && (i - rowStart) < 50 && content[i] != '\n')
            i++;
        rowEnd = i;


        int rowLength = rowEnd - rowStart + 1;
        char* row = malloc((rowLength + 1) * sizeof(char));

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];


        fprintf(file,"%s\n", row);
        

        if(strlen(row) != 50)
            i++;

        free(row);
    }

}

int main(int argc, char** argv) {

    FILE *raport = fopen("pomiar_zad_5.txt", "w");
    if(raport == NULL){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    if(argc < 3) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }


    char* filenameRead = malloc((strlen(argv[1])+1)*sizeof(char));
    char* filenameSave = malloc((strlen(argv[2])+1)*sizeof(char));

    filenameRead = argv[1];
    filenameSave = argv[2];

    FILE* file1 = fopen(filenameRead, "r");
    FILE* file2 = fopen(filenameSave, "w");

    if(file1 == NULL || file2 == NULL) {
        perror("File open error");
        exit(1);
    }


    int fSize = fileSize(file1);
    char* content = malloc((fSize+1)*sizeof(char));


    fread(content,1,fSize,file1);
    
    writeFiftyLetterSentence(file2,content,fSize);
    
    fclose(file1);
    fclose(file2);

    clock_end = times(&tms_end);
    fprintf(raport, "WARIANT 1\n");
    fprintf(raport, "real time: %ld\n", clock_end - clock_start);
    fprintf(raport, "sys time: %ld\n", tms_end.tms_stime - tms_start.tms_stime);
    fprintf(raport ,"user time: %ld\n", tms_end.tms_utime - tms_start.tms_utime);

    fclose(raport);

}