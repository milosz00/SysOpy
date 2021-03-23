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


int rowCounting(int file) {

    int fSize = fileSize(file);
    char* content = malloc((fSize+1)*sizeof(char));

    read(file,content,fSize);

    int count = 0;
    for (int i = 0; i < fSize; i++) {
        if (content[i] == '\n') // Increment count if this character is newline 
            count += 1;
    }
    
    free(content);
    return count;
}

int checkSeventy(int number) {

    number /= 10;
    int tmp = number%10;

    if(tmp == 7)
        return 1;
    return 0;

}

int checkIsPerfectSquare(int number) {
    for (int i = 1; i * i <= number; i++) {
 
        if ((number % i == 0) && (number / i == i)) {
            return 1;
        }
    }
    return 0;
}

void fillArray(int file, int array[], int n) {
    lseek(file,0,SEEK_SET);
    
    int fSize = fileSize(file);
    char* content = malloc((fSize+1)*sizeof(char));

    read(file,content,fSize);
    
    int rowStart,rowEnd, j = 0;
    for(int i = 0; content[i]; i++) {
        rowStart = i;
        while(content[i] && content[i] != '\n') {
            i++;
        }
        rowEnd = i;

        int len = rowEnd - rowStart + 1;
        char* buf = malloc(len*sizeof(char));

        for(int i = rowStart,j=0; i < rowEnd; ++i,++j ) 
            buf[j] = content[i];

        int number = atoi(buf);

        array[j] = number;
        j++;
    }


    free(content);
}

void writeToFile(int file, int number) {
    char buf[100];
    int bytes = sprintf(buf,"%d\n", number);
    write(file,buf,bytes);
}

int main(int argc, char** argv) {

    int raport = open("omiar_zad_3.txt", O_WRONLY | O_APPEND);
    if(raport == -1){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);

    int file = open("data.txt", O_RDONLY);
    int fileToSave1 = open("a.txt", O_CREAT | O_WRONLY);
    int fileToSave2 = open("b.txt", O_CREAT | O_WRONLY);
    int fileToSave3 = open("c.txt", O_CREAT | O_WRONLY);

    int number;
    int rowCount = rowCounting(file);
    int evenCount = 0;

    int numbersFromFile[rowCount];
    fillArray(file,numbersFromFile,rowCount);

    for( int i = 0; i < rowCount; i++ ) {
        number = numbersFromFile[i];

        if(number%2 == 0)
            evenCount++;

        if(number < 10) {
            writeToFile(fileToSave2, number);
        }
        else {
            if(checkSeventy(number)) {
                writeToFile(fileToSave2, number);
            }
        }


        if(checkIsPerfectSquare(number)){
            writeToFile(fileToSave3, number);
        }
    }

    write(fileToSave1,"Liczb parzystych jest: ",23);

    writeToFile(fileToSave1, evenCount);


    close(file);
    close(fileToSave1);
    close(fileToSave2);
    close(fileToSave3);

    clock_end = times(&tms_end);
    write(raport, "\nWARIANT 2\n",11);
    write(raport, "real time: ", 11);
    writeToFile(raport,clock_end - clock_start);
    write(raport, "sys time: ", 10);
    writeToFile(raport,tms_end.tms_stime - tms_start.tms_stime);
    write(raport ,"user time: ", 11);
    writeToFile(raport, tms_end.tms_utime - tms_start.tms_utime);

    close(raport);

}