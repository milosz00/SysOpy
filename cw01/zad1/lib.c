#include "lib.h"


typedef struct node {
    struct node* next;
    char* value;
} node;

#define pushBack(tail, val) \
  *tail = calloc(sizeof(struct node), 1); \
  (*tail)->value = val; \
  tail = &(*tail)->next;


int fileSize(FILE *file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);

    return size;
}

int rowCounting(node** tail, char* content) {

    int rowStart,rowEnd,rowsCount = 0;
    for(int i = 0;content[i];) {
        rowStart = i;
        while(content[i] && content[i] != '\n')
            i++;
        rowEnd = i;

        int rowLength = rowEnd - rowStart + 1;
        char* row = calloc(sizeof(char), rowLength + 1);

        for(int i = rowStart, j = 0; i < rowEnd; ++i, ++j)
            row[j] = content[i];


        i++;

        *tail = calloc(sizeof(struct node), 1); 
        (*tail)->value = row; 
        tail = &(*tail)->next;
        
        rowsCount++;

    }

    return rowsCount;

}

table* createTable(int size) {
  table* t = calloc(sizeof(table), 1);
  t->values = calloc(sizeof(block*), size);
  t->tableSize = size;
  return t;
}

static block* createBlock(int size) {
      block* b = calloc(sizeof(block), 1);
      b->blockSize = size;
      b->rows = calloc(sizeof(char*), size);
      return b;
}

block* mergeTwoFiles(char* filename1, char* filename2) {
    FILE *file1;
    FILE *file2;


    file1 = fopen(filename1, "r");
    file2 = fopen(filename2, "r");


    if(file1 == NULL || file2 == NULL)
        return NULL;


    int fileSize1 = fileSize(file1);
    int fileSize2 = fileSize(file2);


    char* content1 = malloc(fileSize1 + 1);
    char* content2 = malloc(fileSize2 + 1);

    fread(content1, 1, fileSize1, file1);
    fread(content2, 1, fileSize2, file2);
    content1[fileSize1] = 0;
    content2[fileSize2] = 0;
    fclose(file1);
    fclose(file2);

    int rowsCount = 0;

    node* rowList1 = NULL;
    node** tailFirst = &rowList1;

    rowsCount += rowCounting(tailFirst,content1);

    node* rowList2 = NULL;
    node** tailSecond = &rowList2;

    rowsCount += rowCounting(tailSecond,content2);


    block *b = createBlock(rowsCount);
    b->files[0] = filename1;
    b->files[1] = filename2;

    struct node* iter1 = rowList1, *tmp;
    struct node* iter2 = rowList2;

    for (int i = 0; i < rowsCount; i++) {
            if(i%2 == 0){
                b->rows[i] = iter1->value;
                tmp = iter1;
                iter1 = iter1->next;
                free(tmp);
            }
            else {
                b->rows[i] = iter2->value;
                tmp = iter2;
                iter2 = iter2->next;
                free(tmp);
            }
    }

    return b;

}




int mergeFilesSequence(table* table, char* sequence) {
    static char* files[2];

    int size = table->tableSize;
    for(int i = 0; i < size; i++) {
        if(table->values[i])
            continue;
        else {
            files[0] = strtok(sequence,":");
            files[1] = strtok(NULL,"");
            table->values[i] = mergeTwoFiles(files[0],files[1]);
            return i;
        }
    }
    return -1;
}

int rowCount(table* table, int block_index) {
      if (table == NULL || table->values == NULL) return -1;
      block* block = table->values[block_index];
      int count = 0;
      for (int i = 0; i < block->blockSize; i++)
        if (block->rows[i]) count++;
      return count;
}

void removeBlock(table* table, int blockIndex) {
    if (table->values[blockIndex] == NULL) return;
    free(table->values[blockIndex]);
    table->values[blockIndex] = NULL;
}

void removeRowFromBlock(table* table, int blockIndex, int rowIndex) {
      block* block = table->values[blockIndex];

      if (block == NULL) return;
      if(rowIndex > block->blockSize) return;
      if (block->rows[rowIndex] == NULL) return;

      free(block->rows[rowIndex]);
      block->rows[rowIndex] = NULL;

}

void printRows(table* table, int blockIndex) {
    block* block = table->values[blockIndex];

    int size = block->blockSize;
    for(int i = 0; i < size; i++) {
        if(block->rows[i])
            printf("%s\n", block->rows[i]);
    }
}



