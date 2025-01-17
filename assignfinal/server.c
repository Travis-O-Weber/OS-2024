/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX_CREDENTIALS 100  // Define the maximum number of credentials

typedef struct {
    char usernameHash[65];
    char passwordHash[65];
} Credential;

int welcomeSocket, connectionSocket;
int port;

Credential credentials[MAX_CREDENTIALS];
int credentialCount = 0;

void loadCredentials(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }
    while (fscanf(file, "%64s %64s", credentials[credentialCount].usernameHash,
                  credentials[credentialCount].passwordHash) == 2) {
        credentialCount++;
        if (credentialCount >= MAX_CREDENTIALS) break;
    }
    fclose(file);
}

int checkCredentials(const char *usernameHash, const char *passwordHash) {
    int i;  // Move the declaration outside the for loop for C89 compatibility
    for (i = 0; i < credentialCount; i++) {
        if (strcmp(credentials[i].usernameHash, usernameHash) == 0 &&
            strcmp(credentials[i].passwordHash, passwordHash) == 0) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s port-number credentials-file\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    loadCredentials(argv[2]);

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = INADDR_ANY;
    sad.sin_port = htons((u_short)port);

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(welcomeSocket, 10) < 0) {
        perror("Listen failed");
        exit(1);
    }

    while (1) {
        struct sockaddr_in cad;
        socklen_t alen = sizeof(cad);
        connectionSocket = accept(welcomeSocket, (struct sockaddr *)&cad, &alen);
        if (connectionSocket < 0) {
            perror("Accept failed");
            exit(1);
        }

        char clientSentence[128] = {0};
        char buff[128];
        int n = read(connectionSocket, buff, sizeof(buff));
        while (n > 0) {
            strncat(clientSentence, buff, n);
            if (buff[n - 1] == '\0') break;
            n = read(connectionSocket, buff, sizeof(buff));
        }

        char *usernameHash = strtok(clientSentence, " ");
        char *passwordHash = strtok(NULL, " ");

        int found = checkCredentials(usernameHash, passwordHash);
        char capitalizedSentence[128];
        sprintf(capitalizedSentence, "Credential status: %s", found ? "Found" : "Not Found");

        write(connectionSocket, capitalizedSentence, strlen(capitalizedSentence) + 1);

        close(connectionSocket);
    }
}

