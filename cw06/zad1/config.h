#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>
#include <sys/ipc.h>

#define MAX_CLIENTS 5
#define TEXT_LEN 128

typedef enum mtype {
    STOP = 1, DISCONNECT = 2, INIT = 3, LIST = 4, CONNECT = 5
} mtype;


typedef struct client {
    key_t queue_key;
    int client_id;
    int connect_client_id;
} client;

typedef struct msgbuf {
    long mtype;
    char mtext[1024];
    client client;
} msgbuf;


const int SERVER_KEY_ID = 1;
const int MSG_SIZE = sizeof(msgbuf) - sizeof(long);

#endif //CONFIG_H