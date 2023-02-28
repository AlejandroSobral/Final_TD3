#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define PIBES 50

volatile sig_atomic_t child_count = 0; // Global variable to keep track of number of SIGCHLD signals received

void sigchld_handler(int sig) {
    child_count++;
    printf("Entro al Handler de SIGCHLD, aumento contador: %d\n", child_count);
}

int main() {
    int i;
    pid_t pid;

    // Set up signal handler for SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Create child processes
    for (i = 0; i < PIBES; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            sleep(1);
            printf("Child process %d started\n", getpid());
            sleep(1);
            exit(EXIT_SUCCESS);
        }
    }

    // Sleep for a while to simulate doing other work
    printf("Parent process sleeping for 100 seconds...\n");
    sleep(100);
    printf("Soy el padre, me sacaron del sleep!\n");

    // Check if parent process received five SIGCHLD signals
    if (child_count == PIBES) {
        printf("Parent process received five SIGCHLD signals while sleeping\n");
    } else {
        printf("Parent process did not receive five SIGCHLD signals while sleeping\n");
    }

    while(1){
        sleep(1);
    }
    return 0;
}