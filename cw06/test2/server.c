#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>

#include "config.h"

char* clients_queues[MAX_CLIENTS];
int clients_available[MAX_CLIENTS];
int first_free_id = 0;

mqd_t server_queue;


void error_exit(char* msg) {
    printf("Error: %s\n", msg);
    printf("Errno: %d\n", errno);
    exit(EXIT_FAILURE);
}

void quit(int signum) {
    char* msg_res = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients_queues[i] != NULL) {
            mqd_t client_queue_desc = mq_open(clients_queues[i], O_RDWR);
            if(client_queue_desc < 0) error_exit("cannot access client queue");
            if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, STOP) < 0) error_exit("cannot send message");
            if(mq_receive(server_queue, msg_res, MAX_MSG_LEN, NULL) < 0) error_exit("cannot receive message");
            if(mq_close(client_queue_desc) < 0) error_exit("cannot close queue");
        }
    }

    if(mq_close(server_queue) < 0) error_exit("cannot close queue");
    if(mq_unlink(SERVER_QUEUE_NAME) < 0) error_exit("cannot delete queue");
    exit(0);
}

void init_handler(char* msg, char* msg_res) {
    printf("INIT received\n");

    int id = first_free_id++;

    mqd_t client_queue_desc = mq_open(msg, O_RDWR);
    if(client_queue_desc < 0) error_exit("cannot access client queue");

    clients_available[id] = 1;
    clients_queues[id] = (char*)calloc(NAME_LEN, sizeof(char));
    strcpy(clients_queues[id], msg);

    if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, id) < 0) error_exit("cannot send message");
    if(mq_close(client_queue_desc) < 0) error_exit("cannot close queue");
}

void list_handler(char* msg, char* msg_res) {
    printf("LIST received\n");

    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients_queues[i] != NULL) {
            sprintf(msg_res + strlen(msg_res), "ID %d, client %s\n", i, clients_available[i] ? "available" : "not available");
        }
    }

    int client_id = (int) msg[0];
    mqd_t client_queue_desc = mq_open(clients_queues[client_id], O_RDWR);
    if(client_queue_desc < 0) error_exit("cannot access client queue");

    if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, LIST) < 0) error_exit("cannot send message");
    if(mq_close(client_queue_desc) < 0) error_exit("cannot close queue");
}

void connect_handler(char* msg, char* msg_res) {
    printf("CONNECT received\n");
    int client_id = (int) msg[0];
    int other_client_id = (int) msg[1];

    mqd_t client_queue_desc = mq_open(clients_queues[client_id], O_RDWR);
    if(client_queue_desc < 0) error_exit("cannot access client queue");
    msg_res[0] = (char) (other_client_id + 1);
    strcat(msg_res, clients_queues[other_client_id]);
    if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, CONNECT) < 0) error_exit("cannot send message");

    memset(msg_res, 0, strlen(msg_res));
    client_queue_desc = mq_open(clients_queues[other_client_id], O_RDWR);
    if(client_queue_desc < 0) error_exit("cannot access client queue");
    msg_res[0] = (char) (client_id + 1);
    strcat(msg_res, clients_queues[client_id]);
    if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, CONNECT) < 0) error_exit("cannot send message");
    if(mq_close(client_queue_desc) < 0) error_exit("cannot close queue");

    clients_available[client_id] = 0;
    clients_available[other_client_id] = 0;
}

void disconnect_handler(char* msg, char* msg_res) {
    printf("DISCONNECT received\n");
    int client_id = (int) msg[0];
    int other_client_id = (int) msg[1];

    mqd_t client_queue_desc = mq_open(clients_queues[other_client_id], O_RDWR);
    if(client_queue_desc < 0) error_exit("cannot access client queue");
    if(mq_send(client_queue_desc, msg_res, MAX_MSG_LEN, DISCONNECT) < 0) error_exit("cannot send message");
    if(mq_close(client_queue_desc) < 0) error_exit("cannot close queue");

    clients_available[client_id] = 1;
    clients_available[other_client_id] = 1;
}

void stop_handler(char* msg) {
    printf("STOP received\n");
    int client_id = (int) msg[0];

    clients_available[client_id] = 0;
    clients_queues[client_id] = NULL;
}

int main() {
    for(int i = 0; i < MAX_CLIENTS; i++) { clients_queues[i] = NULL; }

    server_queue = create_queue(SERVER_QUEUE_NAME);
    if(server_queue < 0) error_exit("cannot create queue");

    signal(SIGINT, quit);
    char* msg = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    unsigned int prio;

    printf("SERVER START\n");

    while(1) {
        if(mq_receive(server_queue, msg, MAX_MSG_LEN, &prio) < 0) error_exit("cannot receive message");

        char* msg_res = (char*)calloc(MAX_MSG_LEN, sizeof(char));

        switch(prio) {
            case INIT: ;
                init_handler(msg, msg_res);
                break;
            case LIST:
                list_handler(msg, msg_res);
                break;
            case CONNECT:
                connect_handler(msg, msg_res);
                break;
            case DISCONNECT:
                disconnect_handler(msg, msg_res);
                break;
            case STOP:
                stop_handler(msg);
                break;
            default:
                printf("Cannot recognize received message\n");
        }
    }

    return 0;
}