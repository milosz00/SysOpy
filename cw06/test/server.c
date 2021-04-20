#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "config.h"

key_t clients_queues[MAX_CLIENTS];
int clients_available[MAX_CLIENTS];
int first_free_id = 0;

int server_queue;

void quit(int signum) {
    msgbuf* msg_res = malloc(sizeof(msgbuf));
    for(int i = 0; i < MAX_CLIENTS; i++) {
        key_t queue_key = clients_queues[i];
        if(queue_key != -1) {
            msg_res->mtype = STOP;
            int client_queue_id = msgget(queue_key, 0);
            msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);
            msgrcv(server_queue, msg_res, MSG_SIZE, STOP, 0);
        }
    }
    msgctl(server_queue, IPC_RMID, NULL);
    exit(0);
}

void init_handler(msgbuf* msg, msgbuf* msg_res) {
    printf("INIT received\n");
    int id = first_free_id++;

    msg_res->mtype = INIT;
    msg_res->client.client_id = id;

    int client_queue_id = msgget(msg->client.queue_key, 0);

    clients_queues[id] = msg->client.queue_key;
    clients_available[id] = 1;

    msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);
}

void list_handler(msgbuf* msg, msgbuf* msg_res, int client_id) {
    printf("LIST received\n");
    strcpy(msg_res->mtext, "");

    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients_queues[i] != -1) {
            sprintf(msg_res->mtext + strlen(msg_res->mtext), "ID %d, client %s\n", i , clients_available[i] ? "available" : "not available");
        }
    }

    int client_queue_id = msgget(clients_queues[client_id], 0);

    msg_res->mtype = LIST;
    msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);
}

void connect_handler(msgbuf* msg, msgbuf* msg_res, int client_id) {
    printf("CONNECT received\n");
    int other_client_id = msg->client.connect_client_id;

    msg_res->mtype = CONNECT;
    msg_res->client.queue_key = clients_queues[other_client_id];
    int client_queue_id = msgget(clients_queues[client_id], 0);
    msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);

    msg_res->mtype = CONNECT;
    msg_res->client.queue_key = clients_queues[client_id];
    msg_res->client.client_id = client_id;
    client_queue_id = msgget(clients_queues[other_client_id], 0);
    msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);

    clients_available[client_id] = 0;
    clients_available[other_client_id] = 0;
}

void disconnect_handler(msgbuf* msg, msgbuf* msg_res, int client_id) {
    printf("DISCONNECT received\n");
    int other_client_id = msg->client.connect_client_id;

    msg_res->mtype = DISCONNECT;
    int client_queue_id = msgget(clients_queues[other_client_id], 0);
    msgsnd(client_queue_id, msg_res, MSG_SIZE, 0);

    clients_available[client_id] = 1;
    clients_available[other_client_id] = 1;
}

void stop_handler(int client_id) {
    printf("STOP received\n");
    clients_queues[client_id] = -1;
    clients_available[client_id] = 0;
}

int main() {
    for(int i = 0; i < MAX_CLIENTS; i++) { clients_queues[i] = -1; }

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    key_t queue_key = ftok(homedir, SERVER_KEY_ID);
    printf("Server queue key: %d\n", queue_key);
    server_queue = msgget(queue_key, IPC_CREAT | 0666);
    printf("Server queue ID: %d\n", server_queue);

    signal(SIGINT, quit);

    msgbuf* msg = malloc(sizeof(msgbuf));
    while(1) {
        msgrcv(server_queue, msg, MSG_SIZE, -6, 0);

        msgbuf* msg_res = malloc(sizeof(msgbuf));
        int client_id = msg->client.client_id;

        switch(msg->mtype) {
            case INIT: ;
                init_handler(msg, msg_res);
                break;
            case LIST:
                list_handler(msg, msg_res, client_id);
                break;
            case CONNECT:
                connect_handler(msg, msg_res, client_id);
                break;
            case DISCONNECT:
                disconnect_handler(msg, msg_res, client_id);
                break;
            case STOP:
                stop_handler(client_id);
                break;
            default:
                printf("Bad format message\n");
        }
    }

    return 0;
}