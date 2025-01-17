/* client.c - code for example client program that uses TCP */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  // Include for gethostbyname()
#include <unistd.h>
#include "sha256_lib.h"  // Include the header for the SHA-256 library

#define MAX_INPUT_LENGTH 128

int clientSocket;    /* socket descriptor */
int port;            /* protocol port number */

// Function to compute the SHA-256 hash of input
void computeSHA256(char *input, char *output) {
    SHA256_CTX ctx;
    unsigned char hash[SHA256_BLOCK_SIZE];
    int i;  // Declare 'i' here for C89 compatibility

    sha256_init(&ctx);
    sha256_update(&ctx, (unsigned char*)input, strlen(input));
    sha256_final(&ctx, hash);

    for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';  // Null-terminate the string
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s server-name port-number\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_port = htons(port);

    struct hostent *server = gethostbyname(host);
    if (!server) {
        fprintf(stderr, "No such host: %s\n", host);
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    memcpy(&sad.sin_addr.s_addr, server->h_addr, server->h_length);
    if (connect(clientSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        perror("Failed to connect to the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char input[MAX_INPUT_LENGTH];
    char hash[65];  // Buffer to hold the hash output
    char response[1024];
    int choice;

    do {
        printf("\n1: Check username/email\n");
        printf("2: Check password\n");
        printf("3: Check both username/email and password\n");
        printf("4: Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume the newline character after scanf

        if (choice >= 1 && choice <= 3) {
            printf("Enter username/email: ");
            fgets(input, MAX_INPUT_LENGTH, stdin);
            input[strcspn(input, "\n")] = 0; // Remove newline
            computeSHA256(input, hash);
            write(clientSocket, hash, strlen(hash) + 1); // Send the hash

            if (choice == 2 || choice == 3) {
                printf("Enter password: ");
                fgets(input, MAX_INPUT_LENGTH, stdin);
                input[strcspn(input, "\n")] = 0; // Remove newline
                computeSHA256(input, hash);
                write(clientSocket, hash, strlen(hash) + 1); // Send the hash
            }

            int n = read(clientSocket, response, sizeof(response) - 1);
            if (n > 0) {
                response[n] = '\0';
                printf("Server response: %s\n", response);
            } else {
                printf("Error receiving response from server\n");
            }
        }
    } while (choice != 4);

    write(clientSocket, "EXIT", 4);  // Signal server to close connection
    close(clientSocket);
    return 0;
}