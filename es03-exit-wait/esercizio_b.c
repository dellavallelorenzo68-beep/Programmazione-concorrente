#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num1 = 12;
    int num2 = 5;
    int ris = 0;
    int operazione = 2; // 2 = Moltiplicazione (1 = Somma)

    printf("[PADRE] Inizializzazione: num1 = %d, num2 = %d, operazione = %d\n",
           num1, num2, operazione);

    pid_t pid = fork();

    if (pid < 0) {
        perror("[PADRE] Errore nella fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Processo figlio
        printf("  [FIGLIO] Avviato (PID: %d, PPID: %d)\n", getpid(), getppid());

        if (operazione == 1) {
            ris = num1 + num2;
            printf("  [FIGLIO] Esecuzione SOMMA: %d + %d = %d\n", num1, num2, ris);
        } else if (operazione == 2) {
            ris = num1 * num2;
            printf("  [FIGLIO] Esecuzione MOLTIPLICAZIONE: %d * %d = %d\n", num1, num2, ris);
        } else {
            printf("  [FIGLIO] Operazione non valida.\n");
            exit(EXIT_FAILURE);
        }

        printf("  [FIGLIO] Invio risultato %d tramite exit()\n", ris);
        exit(ris);
    } else {
        // Processo padre
        int status;
        printf("[PADRE] In attesa del figlio (PID atteso: %d)...\n", pid);

        pid_t terminated_pid = wait(&status);

        if (terminated_pid < 0) {
            perror("[PADRE] Errore durante la wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("[PADRE] Figlio (PID: %d) terminato. Exit status ricevuto: %d\n",
                   terminated_pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
