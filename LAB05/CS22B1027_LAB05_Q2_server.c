// Server-side Code for Stop-and-Wait ARQ with Manual Acknowledgment and Packet Corruption
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define PORT 8080
#define MAX 1024
#define PACKET_LOSS_PROB 10  // 10% chance of packet loss or corruption

// Function to simulate random packet corruption
bool is_packet_corrupted() {
    int random = rand() % 100;  // Random number between 0 and 99
    return (random < PACKET_LOSS_PROB);  // Corrupt packet with defined probability
}

int main() {
    srand(time(0));  // Seed for random number generator

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connection
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive packets.\n");

    while (1) {
        int bytes_read = read(new_socket, buffer, MAX);
        if (bytes_read <= 0) {
            printf("Connection closed or error.\n");
            break;
        }

        int packet_no;
        sscanf(buffer, "PACKET %d", &packet_no);

        // Simulate packet corruption
        if (is_packet_corrupted()) {
            printf("Packet %d corrupted.\n", packet_no);
            printf("Packet %d is corrupted. No acknowledgment will be sent.\n", packet_no);
        } else {
            printf("Packet %d received correctly.\n", packet_no);
            printf("Enter acknowledgment for packet %d (0 = no acknowledgment, 1 = acknowledgment): ", packet_no);
            int user_ack;
            scanf("%d", &user_ack);
            if (user_ack == 1) {
                // Send acknowledgment
                char ack_message[MAX];
                sprintf(ack_message, "ACK %d", packet_no);
                send(new_socket, ack_message, strlen(ack_message), 0);
                printf("Acknowledgment sent for Packet %d.\n", packet_no);
            } else {
                printf("No acknowledgment sent for Packet %d.\n", packet_no);
            }
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
