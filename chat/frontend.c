#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 1024

void *receive_messages(void *arg);
void show_menu();

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    pthread_t recv_thread;
    char buffer[BUF_SIZE] = {0};
    char message[BUF_SIZE];
    char option[2];

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected to the server.\n");

    pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock);

    while (1) {
        show_menu();
        fgets(option, 2, stdin);
        getchar(); // Consume the newline character left by fgets

        switch (option[0]) {
            case '1':
                printf("Enter your message: ");
                fgets(message, BUF_SIZE, stdin);
                message[strcspn(message, "\n")] = 0; // Remove newline character
                send(sock, message, strlen(message), 0);
                break;
            case '2':
                printf("Quitting the chat...\n");
                close(sock);
                exit(0);
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }

    close(sock);
    return 0;
}

void *receive_messages(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUF_SIZE];
    int valread;

    while ((valread = read(sock, buffer, BUF_SIZE)) > 0) {
        buffer[valread] = '\0';
        printf("\nServer: %s\n", buffer);
        show_menu();
    }

    return NULL;
}

void show_menu() {
    printf("\nMenu:\n");
    printf("1. Send a message\n");
    printf("2. Quit\n");
    printf("Choose an option: ");
}
