#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct command{
    char** arguments;
} command;

#define cmd_number 3
#define arg_number 3


int bufSize(int file) {
    struct stat st;
    fstat(file,&st);
    long size = st.st_size;
    return size;
}

int main(int argc, char** argv) {

    if(argc != 2) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    FILE* file = fopen(argv[1], "r");
    char line[1000];

    while(fgets(line, 999, file) != NULL) {
        char* line_wth_nline = strtok(line, "\n");
        printf("Command: %s\n", line_wth_nline);

        command* commands[cmd_number] = {NULL};
        int cmd_index = 0;

        char* cmd = strtok(line_wth_nline, "|");
        while(cmd != NULL) {
            char** cmd_args = calloc(arg_number + 1, sizeof(char*));

            char* tok_buf;
            char* arg = strtok_r(cmd, " ", &tok_buf); // use strtok_r, because strtok can't use twice at the same time

            cmd_args[0] = strdup(arg);
            int i = 1;

            while((arg = strtok_r(NULL, " ", &tok_buf))) {
                cmd_args[i++] = strdup(arg);
            }

            command* tmp = calloc(1, sizeof(command));
            tmp->arguments = cmd_args;
            commands[cmd_index++] = tmp;

            cmd = strtok(NULL, "|");
        }

        int i;
        int fd[2], fd_prev[2];
        for(i = 0; commands[i]; i++) {
            // create pipe
            pipe(fd);

            pid_t pid = fork();
            if(pid == 0) {
                if(i != 0) { // if not first
                    dup2(fd_prev[0], STDIN_FILENO);
                    close(fd_prev[1]);
                }

                dup2(fd[1], STDOUT_FILENO);
                execvp(commands[i]->arguments[0], commands[i]->arguments);
            }

            close(fd[1]);

            fd_prev[0] = fd[0];
            fd_prev[1] = fd[1];

            for (char** arg = commands[i]->arguments; *arg != NULL; arg++) {
                free(*arg);
            }
            free(commands[i]->arguments);

            free(commands[i]);
        }

        // int bSize = bufSize(fd[0]);
        // printf("%d\n", bSize);
        // char* content = malloc(bSize + 1);
        // read(fd[0], content, bSize);
        // printf("%s", content);

        char buffer[513];
        while (read(fd[0], buffer, sizeof(buffer) - 1) > 0) {
            printf("%s", buffer);
            memset(buffer, 0, sizeof(buffer));
        }

        printf("\n");
        
    } 

    fclose(file);

    return 0;
}