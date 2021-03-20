#ifndef lib
#define lib

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
int mergeFilesSequence(table* table, char* fileSequence);
int rowCount(table* table, int block_index);
void removeBlock(table* table, int block_index);
void removeRowFromBlock(table* table, int blockIndex, int rowIndex);
void printRows(table* table, int blockIndex);

#endif
