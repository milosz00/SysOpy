#ifndef CONFIG_H
#define CONFIG_H

#define MAX_CLIENTS 5
#define NAME_LEN 20

typedef enum mtype {
    STOP = 1, DISCONNECT = 2, INIT = 3, LIST = 4, CONNECT = 5
} mtype;


const int MAX_MSG_LEN = 100;

mqd_t create_queue(const char* name) {
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_CLIENTS;
    attr.mq_msgsize = MAX_MSG_LEN;

    return mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
}

const char* SERVER_QUEUE_NAME = "/SERVER";

#endif //CONFIG_H