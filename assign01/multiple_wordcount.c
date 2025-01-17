#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }// checks to see if file name is provided 

    int successCount = 0, failCount = 0;
    int i;
    for (i = 1; i < argc; i++) 
    {
        pid_t cpid = fork();// creates a new process
        if (cpid == -1) 
        {
            perror("fork");
            continue;
        }
        //check for failure and skip to the next if it fails

        if (cpid == 0) 
        { 
            execl("./wordcount", "wordcount", argv[i], (char *)NULL);
            // If execl() fails
            fprintf(stderr, "Failed to exec wordcount for %s\n", argv[i]);
            exit(2); // Indicate exec failure
        }// Child process
    }

  
    for (i = 1; i < argc; i++) 
    {
        int status;
        wait(&status); // Wait for a child process to exit
        if (WIFEXITED(status)) 
        {
            if (WEXITSTATUS(status) == 0) 
            {
                successCount++;//if succ
            } 
            else if (WEXITSTATUS(status) == 1) 
            {
                failCount++;//if fail
            }
        }// Check if the child exited normally
    }   // Parent process waits for all children

    printf("Parent process %d created %d child processes to count words in %d files\n", getpid(), argc - 1, argc - 1);
    printf("%d files have been counted successfully!\n", successCount);
    printf("%d files did not exist\n", failCount);
  //printline statyements

    return 0;
}