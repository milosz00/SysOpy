#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR); // create named pipe

    // producers run
    if (fork() == 0) {
        execlp("./producer", "./producer", FIFO_NAME, "first.txt", "0", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", FIFO_NAME, "second.txt", "1", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", FIFO_NAME, "third.txt", "2", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", FIFO_NAME, "fourth.txt", "3", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", FIFO_NAME, "fifth.txt", "4", "10", NULL);
    }

    // consumer run
    if (fork() == 0) {
        execlp("./consumer", "./consumer", FIFO_NAME, "result.txt", "10", NULL);
    }


    for (int i = 0; i < 6; i++) {
        wait(NULL); // wait for process terminated
    }
}