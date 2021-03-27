#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include "lib.h"

struct tms tmsStart, tmsEnd;
clock_t clockStart, clockEnd;

int main(int argc, char** argv) {

    table* container = NULL;

    FILE* file = fopen("result.txt", "a");

    for(int i = 1; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg,"create_table") == 0) {
            if(container)
                free(container);
            int tableSize = atoi(argv[++i]);
            container = createTable(tableSize);
        }
        else if(strcmp(arg,"merge_files") == 0) {

            int j = i + 1;
            char* filesToMerge = argv[j];
            pid_t pid;

		    clockStart = times(&tmsStart);
            while(strchr(filesToMerge,':') && j < argc) {
                switch (pid = vfork())
                {
                    case -1:
                        fprintf(stderr, "Error in fork\n");
                        exit(1);
                        break;

                    case 0:
                        printf("Tutaj jestem, proces potomny: %d\n", getpid());
                        mergeFilesSequence(container,filesToMerge, getpid());
                        kill(getpid(), SIGTERM);
                        break;

                    default:
                        break;
                }
                j++;

                if(j < argc) {
                    filesToMerge = argv[j];
                }
            }

            clockEnd = times(&tmsEnd);
            fprintf(file,"MERGE FILES OPERATION\n--------------------\n");
            fprintf(file,"real time: %ld\n", clockEnd - clockStart);
            fprintf(file,"sys time: %ld\n", tmsEnd.tms_stime - tmsStart.tms_stime);
            fprintf(file,"user time: %ld\n", tmsEnd.tms_utime - tmsStart.tms_utime);

            i = i + container->tableSize;
        }
        else if(strcmp(arg,"print_block") == 0) {
            int blockIndex = atoi(argv[++i]);
            printRows(container,blockIndex);
        }

    }



    fclose(file);
    return 0;
}

