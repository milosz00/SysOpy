#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;

int rowCounting(FILE *file) {

    char c;
    int count = 0;
    for (c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') // Increment count if this character is newline 
            count += 1;
    }
  
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

int main(int argc, char** argv) {

    FILE *raport = fopen("pomiar_zad_3.txt", "w");
    if(raport == NULL){
        perror("File open error");
        exit(1);
    }
    clock_start = times(&tms_start);


    FILE* file = fopen("data.txt", "r");
    FILE* fileToSave1 = fopen("a.txt", "a");
    FILE* fileToSave2 = fopen("b.txt", "a");
    FILE* fileToSave3 = fopen("c.txt", "a");

    int number;
    int rowCount = rowCounting(file);
    int evenCount = 0;

    fseek(file,0,SEEK_SET);
    for( int i = 0; i < rowCount; i++ ) {
        fscanf(file,"%d",&number);
        if(number%2 == 0)
            evenCount++;

        if(number < 10)
            fprintf(fileToSave2,"%d\n",number);
        else {
            if(checkSeventy(number))
                fprintf(fileToSave2,"%d\n",number);
        }


        if(checkIsPerfectSquare(number))
            fprintf(fileToSave3,"%d\n",number);
    }

    fprintf(fileToSave1, "Liczb parzystych jest %d\n", evenCount);


    fclose(file);
    fclose(fileToSave1);
    fclose(fileToSave2);
    fclose(fileToSave3);

    clock_end = times(&tms_end);
    fprintf(raport, "WARIANT 1\n");
    fprintf(raport, "real time: %ld\n", clock_end - clock_start);
    fprintf(raport, "sys time: %ld\n", tms_end.tms_stime - tms_start.tms_stime);
    fprintf(raport ,"user time: %ld\n", tms_end.tms_utime - tms_start.tms_utime);

    fclose(raport);

}