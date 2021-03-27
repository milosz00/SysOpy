#ifndef lib
#define lib

//#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct block {
    int blockSize;
    char* files[2];
    char** rows;
} block;


typedef struct table {
    int tableSize;
    block** values;
} table;


table* createTable(int tableSize);



block* mergeTwoFiles(char* file1, char* file2);
int mergeFilesSequence(table* table, char* fileSequence, pid_t pid);
int rowCount(table* table, int block_index);
void removeBlock(table* table, int block_index);
void removeRowFromBlock(table* table, int blockIndex, int rowIndex);
void printRows(table* table, int blockIndex);

#endif
