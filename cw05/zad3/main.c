#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/myfifo"

void run_child(char* path, int producer, char* producer_number) {
    if (fork() == 0) {
        if (producer) {
            execlp("./producer", "./producer", FIFO_NAME, path, producer_number, "10", NULL);
        } else {
            execlp("./consumer", "./consumer", FIFO_NAME, path, "10", NULL);
        }
    }
}

int main() {
    mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR);

    run_child("1.txt", 1, "0");
    run_child("A.txt", 1, "1");
    run_child("B.txt", 1, "2");
    run_child("p.txt", 1, "3");
    run_child("d.txt", 1, "4");
    run_child("out.txt", 0, "-1");

    for (int i = 0; i < 6; i++) {
        wait(NULL);
    }
}