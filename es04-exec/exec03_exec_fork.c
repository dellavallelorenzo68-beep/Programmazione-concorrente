#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int status;
    printf("\n[INIZIO] Prima della fork pid = %d, pid padre = %d\n\n", getpid(), getppid());

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore durante fork");
        exit(1);
    }

    if (pid == 0) {
        printf("[Figlio] pid = %d, pid padre = %d\n", getpid(), getppid());
        execl("/bin/ls", "ls", "-a", (char *)0);
        perror("exec fallita!");
        exit(112);
    } else {
        pid_t atteso = wait(&status);
        printf("[Padre] wait_retvalue = %d, exit_status = %d\n",
               atteso, WIFEXITED(status) ? WEXITSTATUS(status) : -1);
        printf("[FINE] Termine del processo con pid = %d avente pid padre = %d\n", getpid(), getppid());
    }

    return 0;
}
