#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    printf("[PADRE] Avvio processo principale (PID: %d)\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("Errore durante la fork");
        exit(1);
    }

    if (pid == 0) {
        // Processo figlio
        int exit_code = 253;
        printf("  [FIGLIO] In esecuzione (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("  [FIGLIO] Terminazione volontaria con exit(%d)...\n", exit_code);
        exit(exit_code);
    } else {
        // Processo padre
        int status;
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("[PADRE] Figlio (PID: %d) terminato correttamente.\n", child_pid);
            printf("[PADRE] Exit status estratto: %d\n", WEXITSTATUS(status));
        }
    }

    // Test della wait senza figli attivi
    printf("[PADRE] Test wait() senza figli attivi:\n");
    pid_t err_wait = wait(NULL);
    if (err_wait < 0) {
        perror("[PADRE] Esito atteso");
    }

    return 0;
}
