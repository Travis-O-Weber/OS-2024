# Secure Client-Server Authentication System

## Overview
This project implements a client-server authentication system using TCP sockets and SHA-256 hashing. The client sends username and password hashes to the server, which verifies them against a stored credentials list.

## Files in the Project

- **`server.c`**: Implements the authentication server.
- **`client.c`**: Implements the authentication client.
- **`sha256_lib.c`** & **`sha256_lib.h`**: Implements SHA-256 hashing functions.
- **`Makefile`**: Used to compile the project.

## Function Descriptions

### `server.c`
This file implements a TCP server that listens for incoming connections and verifies hashed credentials.

#### **Functions**
- **`void loadCredentials(const char *filename)`**
  - Loads stored username and password hashes from a file.
- **`int checkCredentials(const char *usernameHash, const char *passwordHash)`**
  - Checks if the received username and password hashes match any stored credentials.
- **`int main(int argc, char *argv[])`**
  - Initializes the server, accepts client connections, and processes authentication requests.

### `client.c`
This file implements a TCP client that hashes user input and sends authentication requests to the server.

#### **Functions**
- **`void computeSHA256(char *input, char *output)`**
  - Computes the SHA-256 hash of the given input.
- **`int main(int argc, char *argv[])`**
  - Connects to the server, prompts the user for credentials, and sends authentication requests.

### `sha256_lib.c`
Implements SHA-256 hashing for secure password storage and transmission.

#### **Functions**
- **`void sha256_init(SHA256_CTX *ctx)`**
  - Initializes SHA-256 context.
- **`void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len)`**
  - Updates the SHA-256 hash with new data.
- **`void sha256_final(SHA256_CTX *ctx, uint8_t hash[])`**
  - Finalizes the hash and produces the digest.

## How to Compile
```sh
make
```

## How to Run
### Start the Server
```sh
./server <port> <credentials-file>
```
Example:
```sh
./server 8080 credentials.txt
```

### Start the Client
```sh
./client <server-address> <port>
```
Example:
```sh
./client localhost 8080
```

## Notes
- The **server** listens for client connections and verifies SHA-256 hashed credentials.
- The **client** hashes user input before sending it to the server.
- The **SHA-256 library** ensures secure hashing of credentials before transmission.
