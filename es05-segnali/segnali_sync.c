#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t segnale_ricevuto = 0;

void gestore_usr1(int sig) {
    (void)sig;
    segnale_ricevuto = 1;
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = gestore_usr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("Errore sigaction");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        return 1;
    }

    if (pid == 0) {
        printf("[Figlio %d] In attesa di SIGUSR1 dal padre (%d)...\n", getpid(), getppid());
        while (!segnale_ricevuto) {
            pause();
        }
        printf("[Figlio %d] Segnale SIGUSR1 ricevuto! Procedo con l'attivita'.\n", getpid());
        exit(0);
    } else {
        sleep(1);
        printf("[Padre %d] Invio SIGUSR1 al figlio (%d)...\n", getpid(), pid);
        if (kill(pid, SIGUSR1) < 0) {
            perror("Errore kill");
            return 1;
        }

        int status;
        wait(&status);
        printf("[Padre %d] Figlio terminato con stato %d.\n",
               getpid(), WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    }

    return 0;
}
