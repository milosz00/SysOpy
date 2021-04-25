#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "config.h"

key_t queue_key;
int queue_id;
int server_queue_id;
int client_id;

void stop() {
    msgbuf* msg = malloc(sizeof(msgbuf));
    msg->mtype = STOP;
    msg->client.client_id = client_id;

    msgsnd(server_queue_id, msg, MSG_SIZE, 0);
    msgctl(queue_id, IPC_RMID, NULL);

    msgctl(queue_id, IPC_RMID, NULL);
    exit(0);
}

void chat_mode(int other_id, int other_queue_id) {
    char* cmd = NULL;
    size_t len = 0;
    ssize_t read = 0;
    msgbuf* msg = malloc(sizeof(msgbuf));
    while(1) {
        printf("Enter message or DISCONNECT: ");
        read = getline(&cmd, &len, stdin);
        cmd[read - 1] = '\0';

        if(msgrcv(queue_id, msg, MSG_SIZE, STOP, IPC_NOWAIT) >= 0) {
            printf("STOP from server, quitting...\n");
            stop();
        }

        if(msgrcv(queue_id, msg, MSG_SIZE, DISCONNECT, IPC_NOWAIT) >= 0) {
            printf("Disconnecting...\n");
            break;
        }

        while(msgrcv(queue_id, msg, MSG_SIZE, 0, IPC_NOWAIT) >= 0) {
            printf("[%d]: %s\n", other_id, msg->mtext);
        }

        if(strcmp(cmd, "DISCONNECT") == 0) {
            msg->mtype = DISCONNECT;
            msg->client.client_id = client_id;
            msg->client.connect_client_id = other_id;
            msgsnd(server_queue_id, msg, MSG_SIZE, 0);
            break;
        } else if(strcmp(cmd, "") != 0) {
            msg->mtype = CONNECT;
            strcpy(msg->mtext, cmd);
            msgsnd(other_queue_id, msg, MSG_SIZE, 0);
        }
    }
}


int init() {
    msgbuf* msg = malloc(sizeof(msgbuf));
    msg->mtype = INIT;
    msg->client.queue_key = queue_key;
    msgsnd(server_queue_id, msg, MSG_SIZE, 0);

    msgbuf* msg_rcv = malloc(sizeof(msgbuf));
    msgrcv(queue_id, msg_rcv, MSG_SIZE, 0, 0);

    int client_id = msg_rcv->client.client_id;
    return client_id;
}

void connect_command(int id_to_connect) {
    msgbuf* msg = malloc(sizeof(msgbuf));
    msg->mtype = CONNECT;
    msg->client.client_id = client_id;
    msg->client.connect_client_id = id_to_connect;

    msgsnd(server_queue_id, msg, MSG_SIZE, 0);

    msgbuf* msg_rcv = malloc(sizeof(msgbuf));
    msgrcv(queue_id, msg_rcv, MSG_SIZE, 0, 0);

    key_t other_queue_key = msg_rcv->client.queue_key;
    int other_queue_id = msgget(other_queue_key, 0);

    chat_mode(id_to_connect, other_queue_id);
}

void list_command() {
    msgbuf* msg = malloc(sizeof(msgbuf));
    msg->mtype = LIST;
    msg->client.client_id = client_id;
    msgsnd(server_queue_id, msg, MSG_SIZE, 0);

    msgbuf* msg_rcv = malloc(sizeof(msgbuf));
    msgrcv(queue_id, msg_rcv, MSG_SIZE, 0, 0);
    printf("%s\n", msg_rcv->mtext);
}

void quit(int signum) {
    stop();
}

void check_server_message() {
    msgbuf* msg = malloc(sizeof(msgbuf));

    if(msgrcv(queue_id, msg, MSG_SIZE, 0, IPC_NOWAIT) >= 0) {
        if(msg->mtype == STOP) {
            printf("STOP from server, quitting...\n");
            stop();
        } else if(msg->mtype == CONNECT) {
            printf("Connecting to client %d...\n", msg->client.client_id);
            int other_queue_id = msgget(msg->client.queue_key, 0);
            chat_mode(msg->client.client_id, other_queue_id);
        }
    }
}


int random_number() {
    return rand() % 255 + 1;
}

int main() {
    srand(time(NULL));

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    queue_key = ftok(homedir, random_number());
    printf("Queue key: %d\n", queue_key);

    queue_id = msgget(queue_key, IPC_CREAT | 0666);
    printf("Queue ID: %d\n", queue_id);

    key_t server_key = ftok(homedir, SERVER_KEY_ID);
    server_queue_id = msgget(server_key, 0);
    printf("Server queue ID: %d\n", server_queue_id);

    client_id = init();
    printf("ID received: %d\n", client_id);

    signal(SIGINT, quit);

    char* command = NULL;
    size_t len = 0;
    ssize_t read = 0;
    while(1) {
        printf("Enter the command: ");
        read = getline(&command, &len, stdin);  // getline reads to the new line
        command[read - 1] = '\0';

        check_server_message();

        if(strcmp(command, "") == 0) {
            continue;
        }

        char* tok = strtok(command, " ");
        if(strcmp(tok, "LIST") == 0) {
            printf("LIST command\n");
            list_command();
        } else if(strcmp(tok, "CONNECT") == 0) {
            printf("CONNECT command\n");
            tok = strtok(NULL, " ");
            int id_to_connect = atoi(tok);
            connect_command(id_to_connect);
        } else if(strcmp(tok, "STOP") == 0) {
            printf("STOP command\n");
            stop();
        } else {
            printf("Bad format of command: %s\n", command);
        }
    }

    return 0;
}