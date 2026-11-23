#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Errore durante fork");
        return 1;
    }

    if (pid == 0) {
        execl("./somma", "somma", "15", "27", (char *)0);
        perror("Errore durante execl");
        exit(1);
    } else {
        int status;
        if (wait(&status) < 0) {
            perror("Errore durante wait");
            return 1;
        }
        printf("Calcolo completato\n");
    }

    return 0;
}
