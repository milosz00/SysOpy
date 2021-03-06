#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>
#include <time.h>

#include "config.h"

char queue_name[NAME_LEN];

mqd_t queue_desc;
mqd_t server_queue_desc;
int client_id;


void stop_command() {
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    msg[0] = client_id;

    if(mq_send(server_queue_desc, msg, MAX_MSG_LEN, STOP) < 0) error_exit("cannot send message");
    if(mq_close(server_queue_desc) < 0) error_exit("cannot close queue");
    exit(0);
}


void quit(int signum) {
    stop_command();
}


void chat_mode(int other_id, mqd_t other_queue_desc) {
    char* cmd = NULL;
    size_t len = 0;
    ssize_t read = 0;
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    while(1) {
        printf("Enter message or DISCONNECT: ");
        read = getline(&cmd, &len, stdin);
        cmd[read - 1] = '\0';

        struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
        unsigned int type;
        int disconnect = 0;
        while(mq_timedreceive(queue_desc, msg, MAX_MSG_LEN, &type, tspec) >= 0) {
            if(type == STOP) {
                printf("STOP from server, quitting...\n");
                stop_command();
            } else if(type == DISCONNECT) {
                printf("Disconnecting...\n");
                disconnect = 1;
                break;
            } else {
                printf("[%d]: %s\n", other_id, msg);
            }
        }

        if(disconnect) break;

        if(strcmp(cmd, "DISCONNECT") == 0) {
            msg[0] = client_id;
            msg[1] = other_id;
            if(mq_send(server_queue_desc, msg, MAX_MSG_LEN, DISCONNECT) < 0) error_exit("cannot send message");
            break;
        } else if(strcmp(cmd, "") != 0) {
            strcpy(msg, cmd);
            if(mq_send(other_queue_desc, msg, MAX_MSG_LEN, CONNECT) < 0) error_exit("cannot send message");
        }
    }
}


int init() {
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    strcpy(msg, queue_name);

    if(mq_send(server_queue_desc, msg, MAX_MSG_LEN, INIT) < 0) error_exit("cannot send message");

    unsigned int client_id;
    if(mq_receive(queue_desc, msg, MAX_MSG_LEN, &client_id) < 0) error_exit("cannot receive message");
    printf("ID received %d\n", client_id);

    return client_id;
}

void list_command() {
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    msg[0] = client_id;

    if(mq_send(server_queue_desc, msg, MAX_MSG_LEN, LIST) < 0) error_exit("cannot send message");

    if(mq_receive(queue_desc, msg, MAX_MSG_LEN, NULL) < 0) error_exit("cannot receive message");

    printf("%s\n", msg);
}

void connect_command(int id) {
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    msg[0] = client_id;
    msg[1] = id;

    if(mq_send(server_queue_desc, msg, MAX_MSG_LEN, CONNECT) < 0) error_exit("cannot send message");

    if(mq_receive(queue_desc, msg, MAX_MSG_LEN, NULL) < 0) error_exit("cannot receive message");

    char* other_queue_name = (char*)calloc(NAME_LEN, sizeof(char));
    strncpy(other_queue_name, msg + 1, strlen(msg) - 1);
    printf("------------------\n");
    mqd_t other_queue_desc = mq_open(other_queue_name, O_RDWR);


    chat_mode(id, other_queue_desc);
}


void check_server_message() {
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));

    struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
    unsigned int type;
    if(mq_timedreceive(queue_desc, msg, MAX_MSG_LEN, &type, tspec) >= 0) {
        if(type == STOP) {
            printf("STOP from server, quitting...\n");
            stop_command();
        } else if(type == CONNECT) {
            printf("Connecting to client...\n");

            char* other_queue_name = (char*)calloc(NAME_LEN, sizeof(char));
            strncpy(other_queue_name, msg + 1, strlen(msg) - 1);
            printf("other name %s\n", other_queue_name);
            mqd_t other_queue_desc = mq_open(other_queue_name, O_RDWR);

            chat_mode((int) msg[0] - 1, other_queue_desc);
        }
    }
}


void generate_name() {
    queue_name[0] = '/';
    for(int i = 1; i < NAME_LEN; i++) queue_name[i] = random_char();
}

int main(int argc, char** argv) {
    srand(time(NULL));
    generate_name();
    printf("Queue name %s\n", queue_name);

    queue_desc = create_queue(queue_name);

    server_queue_desc = mq_open(SERVER_QUEUE_NAME, O_RDWR, 0666, NULL);

    client_id = init();

    signal(SIGINT, quit);

    while(1) {
        printf("Enter command: ");
        char* cmd = NULL;
        size_t len = 0;
        ssize_t read = 0;
        read = getline(&cmd, &len, stdin);
        cmd[read - 1] = '\0';

        check_server_message();

        if(strcmp(cmd, "") == 0) {
            continue;
        }

        char* tok = strtok(cmd, " ");
        if(strcmp(tok, "LIST") == 0) {
            printf("LIST command\n");
            list_command();
        } else if(strcmp(tok, "CONNECT") == 0) {
            tok = strtok(NULL, " ");
            int id = atoi(tok);
            connect_command(id);
        } else if(strcmp(tok, "STOP") == 0) {
            printf("STOP command\n");
            stop_command();
        } else {
            printf("Unrecognized command: %s\n", cmd);
        }

    }

    return 0;
}