#define _DEFAULT_SOURCE
#include <string.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>



int fileSize(FILE *file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);

    return size;
}

const char* get_file_ext(const char *filename) {

    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int searchForSentenceInFile(char* filename, char* sentenceWanted) {
    FILE* file = fopen(filename, "r");

    if(file == NULL) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }

    int fSize = fileSize(file);
    char* content = malloc(sizeof(char) * (fSize + 1));
    fread(content, 1, fSize, file);
    fclose(file);

    int strLen = strlen(sentenceWanted);

    int rowStart,rowEnd;
    for(int i = 0;content[i];) {
        rowStart = i;
        while(content[i] && (i - rowStart) < strLen && content[i] != '\n')
            i++;
        rowEnd = i;

        int rowLength = rowEnd - rowStart + 1;
        char* row = malloc((rowLength + 1) * sizeof(char));

        int j = 0;
        for(int i = rowStart; i < rowEnd; ++i, ++j) {
            row[j] = content[i];
        }
        row[j] = '\0';
        

        if(strcmp(sentenceWanted, row) == 0) {
            return 1;
        }
        else 
            i = rowStart + 1;    
        free(row);
    }

    return 0;
}


void listdir(const char *name, int indent, char* sentenceWanted, int depth)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)) || depth == 0)
        return;

    pid_t pid;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);  // we add name of file or directory at the end of path name
        stat(path, &st);

        if (S_ISDIR(st.st_mode) && (strcmp(entry->d_name, ".") && (strcmp(entry->d_name, "..")))) { // we don't want go to files, which are in . or .. directory

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            switch (pid = vfork())  
            {
                case -1:
                    fprintf(stderr, "Error in fork\n");
                    exit(1);
                    break;
                case 0:
                    listdir(path, indent + 2, sentenceWanted,depth-1);
                    kill(getpid(), SIGTERM);
                    break;
            
                default:
                    break;
            }
        } else if(strcmp(get_file_ext(entry->d_name), "txt") == 0){

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", name, entry -> d_name);

            if(searchForSentenceInFile(path,sentenceWanted)){
                printf("Proces macierzysty PID: %d\n", getppid());
                printf("Proces potomny PID: %d\n", getpid());
                printf("\n%s\n\n", path);
            }
        }

    }
    closedir(dir);
}


int main(int argc, char** argv) { 


    if(argc < 4) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    char* dirName = malloc(sizeof(char) * (strlen(argv[1]) + 1) );
    char* sentenceWanted = malloc(sizeof(char) * (strlen(argv[2]) + 1) );
    char* depthNumber = malloc(sizeof(char) * (strlen(argv[3]) + 1));

    dirName = argv[1];
    sentenceWanted = argv[2];
    depthNumber = argv[3];

    int depth = atoi(depthNumber);
    

    listdir(dirName,0,sentenceWanted,depth);
   
    return 0; 
} 