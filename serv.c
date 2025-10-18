#include <netinet/in.h> // for sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h> // for close()
#include "list.h"

#define PORT 9001
#define ACK "ACK"
#define BUF_SIZE 1024

int servSockD, clientSocket;
list_t *mylist;

// Graceful shutdown on Ctrl+C
void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    if (mylist) {
        list_free(mylist);
    }
    if (clientSocket > 0) close(clientSocket);
    if (servSockD > 0) close(servSockD);
    exit(0);
}

int main(int argc, char const *argv[]) {
    int n, val, idx;
    char buf[BUF_SIZE];
    char sbuf[BUF_SIZE];
    char *token;

    struct sockaddr_in servAddr;

    // Register signal handler
    signal(SIGINT, handle_sigint);

    // Create socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for client on port %d...\n", PORT);

    // Accept connection
    clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Create linked list
    mylist = list_alloc();

    while (1) {
        memset(buf, 0, BUF_SIZE);
        memset(sbuf, 0, BUF_SIZE);

        // Receive from client
        n = recv(clientSocket, buf, sizeof(buf), 0);
        if (n <= 0) {
            printf("Client disconnected or error.\n");
            break;
        }

        buf[n] = '\0';
        token = strtok(buf, " ");

        if (token == NULL) continue;

        // Command processing
        if (strcmp(token, "exit") == 0) {
            list_free(mylist);
            sprintf(sbuf, "Server shutting down...");
            send(clientSocket, sbuf, sizeof(sbuf), 0);
            break;
        } else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            sprintf(sbuf, "Length = %d", val);
        } else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_front(mylist, val);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_back(mylist, val);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "add_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_at_index(mylist, val, idx);
            sprintf(sbuf, "%s%d@%d", ACK, val, idx);
        } else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_remove_at_index(mylist, idx);
            sprintf(sbuf, "%s%d", ACK, val);
        } else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_get_elem_at(mylist, idx);
            sprintf(sbuf, "Element at index %d = %d", idx, val);
        } else if (strcmp(token, "print") == 0) {
            char *list_str = listToString(mylist);
            snprintf(sbuf, BUF_SIZE, "%s", list_str);
            free(list_str);
        } else {
            snprintf(sbuf, BUF_SIZE, "Unknown command.");
        }

        // Send response
        send(clientSocket, sbuf, sizeof(sbuf), 0);
    }

    // Cleanup
    if (mylist) list_free(mylist);
    close(clientSocket);
    close(servSockD);

    return 0;
}
