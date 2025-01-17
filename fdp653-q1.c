/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Flag to indicate if SIGUSR2 has been received
volatile sig_atomic_t sigusr2_received = 0;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    static int count = 0;
    count++;
    printf("Caught SIGINT %d time(s)\n", count);
}

// Signal handler for SIGUSR2
void handle_sigusr2(int sig) {
    sigusr2_received = 1;
}

int main() {
    // Print process id (PID)
    printf("Process ID is %d\n", getpid());

    // Block the SIGUSR1 signal
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // Set up the SIGINT (Ctrl+C) signal handler
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("Program starts waiting for SIGUSR2...\n");

    // Set up the SIGUSR2 signal handler
    struct sigaction sa2;
    sa2.sa_handler = handle_sigusr2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIGUSR2, &sa2, NULL);

    // Block SIGUSR2 before entering the loop to avoid missing the signal
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    // Check flag in a while loop using sigsuspend
    while (!sigusr2_received) {
        sigsuspend(&oldmask);
    }

    printf("Got SIGUSR2, terminating the program.\n");

    // Restore the oldmask which was obtained when blocking SIGUSR2
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    return 0;
}