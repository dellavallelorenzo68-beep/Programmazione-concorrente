#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    char cmd[80];
    char scelta[16];

    for (;;) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Errore durante fork");
            exit(1);
        }

        if (pid == 0) {
            printf("Inserire il comando da eseguire: ");
            if (scanf("%79s", cmd) != 1) {
                exit(1);
            }
            execlp(cmd, cmd, (char *)0);
            perror("Errore esecuzione comando");
            exit(1);
        } else {
            int status;
            if (wait(&status) < 0) {
                perror("Errore durante wait");
                exit(1);
            }

            printf("Eseguire altro comando? (si/no): ");
            if (scanf("%15s", scelta) != 1 || strcmp(scelta, "si") != 0) {
                break;
            }
        }
    }

    return 0;
}
