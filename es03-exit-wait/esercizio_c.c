#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num1 = 15;
    int num2 = 6;
    int ris = 0;

    printf("[PADRE] Avvio (PID: %d) - num1 = %d, num2 = %d\n", getpid(), num1, num2);

    // Creazione del primo figlio (Somma)
    pid_t pid1 = fork();

    if (pid1 < 0) {
        perror("[PADRE] Errore prima fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Figlio 1: Somma
        ris = num1 + num2;
        printf("  [FIGLIO 1 - SOMMA] PID: %d, PPID: %d -> %d + %d = %d\n",
               getpid(), getppid(), num1, num2, ris);
        exit(ris);
    }

    // Creazione del secondo figlio eseguita solo dal padre (Moltiplicazione)
    pid_t pid2 = fork();

    if (pid2 < 0) {
        perror("[PADRE] Errore seconda fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Figlio 2: Moltiplicazione
        ris = num1 * num2;
        printf("  [FIGLIO 2 - MOLTIPLICAZIONE] PID: %d, PPID: %d -> %d * %d = %d\n",
               getpid(), getppid(), num1, num2, ris);
        exit(ris);
    }

    // Processo padre
    printf("[PADRE] Creati Figlio 1 (PID: %d) e Figlio 2 (PID: %d)\n", pid1, pid2);
    printf("[PADRE] Attesa dei figli in corso...\n");

    int status;
    for (int i = 0; i < 2; i++) {
        pid_t terminated_pid = wait(&status);

        if (terminated_pid < 0) {
            perror("[PADRE] Errore wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            int valore_ritornato = WEXITSTATUS(status);
            if (terminated_pid == pid1) {
                printf("[PADRE] Rilevata terminazione Figlio 1 (SOMMA) -> Risultato = %d\n",
                       valore_ritornato);
            } else if (terminated_pid == pid2) {
                printf("[PADRE] Rilevata terminazione Figlio 2 (MOLTIPLICAZIONE) -> Risultato = %d\n",
                       valore_ritornato);
            }
        }
    }

    printf("[PADRE] Tutti i processi figli hanno terminato l'esecuzione.\n");
    return 0;
}
