#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include "lib.h"


#ifdef dynamic 
#include <dlfcn.h>
#endif


struct tms tmsStart, tmsEnd;
clock_t clockStart, clockEnd;


double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char** argv) {

    #ifdef dynamic
        void* handle = dlopen("./liblib.so", RTLD_NOW);
        if( !handle ) {
            fprintf(stderr, "dlopen() %s\n", dlerror());
            exit(1);
        }
        table* (*createTable)(int tableSize) = dlsym(handle, "createTable");
        int (*mergeFilesSequence)(table* table, char* fileSequence) = dlsym(handle, "mergeFilesSequence");
        void (*removeBlock)(table*, int) = dlsym(handle, "removeBlock");
        void (*removeRowFromBlock)(table*, int, int) = dlsym(handle, "removeRowFromBlock");
        int (*rowCount)(table* table, int block_index) = dlsym(handle, "rowCount");
        void (*printRows)(table* table, int blockIndex) = dlsym(handle,"printRows");
    #endif




    table* container = NULL;


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

		    clockStart = times(&tmsStart);
            while(strchr(filesToMerge,':') && j < argc) {
                mergeFilesSequence(container,filesToMerge);
                j++;

                if(j < argc) {
                    filesToMerge = argv[j];
                }
            }

            clockEnd = times(&tmsEnd);
            printf("MERGE FILES OPERATION\n--------------------\n");
            printf("real time: %f\n", time_in_seconds(clockStart,clockEnd));
            printf("sys time: %f\n", time_in_seconds(tmsStart.tms_stime,tmsEnd.tms_stime));
            printf("user time: %f\n", time_in_seconds(tmsStart.tms_utime,tmsEnd.tms_utime));

            i = i + container->tableSize;
        }
        else if(strcmp(arg, "remove_block") == 0) {

            int blockIndex = atoi(argv[++i]);

            clockStart = times(&tmsStart);

            removeBlock(container, blockIndex);

            clockEnd = times(&tmsEnd);
            printf("REMOVE BLOCK OPERATION\n--------------------\n");
            printf("real time: %f\n", time_in_seconds(clockStart,clockEnd));
            printf("sys time: %f\n", time_in_seconds(tmsStart.tms_stime,tmsEnd.tms_stime));
            printf("user time: %f\n", time_in_seconds(tmsStart.tms_utime,tmsEnd.tms_utime));
        }
        else if(strcmp(arg, "remove_row") == 0) {

            int blockIndex = atoi(argv[++i]);
            int rowIndex = atoi(argv[++i]);

            clockStart = times(&tmsStart);

            removeRowFromBlock(container,blockIndex,rowIndex);

            clockEnd = times(&tmsEnd);
            printf("REMOVE ROW OPEARTION\n--------------------\n");
            printf("real time: %f\n", time_in_seconds(clockStart,clockEnd));
            printf("sys time: %f\n", time_in_seconds(tmsStart.tms_stime,tmsEnd.tms_stime));
            printf("user time: %f\n", time_in_seconds(tmsStart.tms_utime,tmsEnd.tms_utime));

        }
        else if(strcmp(arg, "row_count") == 0) {

            int blockIndex = atoi(argv[++i]);
            
            int rowsNumber = rowCount(container, blockIndex);
            printf("In block %d is %d rows\n",blockIndex,rowsNumber);

        }
        else if(strcmp(arg,"print_block") == 0) {
            int blockIndex = atoi(argv[++i]);
            printRows(container,blockIndex);
        }


    }

    return 0;
}
