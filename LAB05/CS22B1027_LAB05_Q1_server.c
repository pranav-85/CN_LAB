// Server-side Code for Byte Stuffing using Hexadecimal
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024
#define FLAG_BYTE 0x7E
#define ESCAPE_BYTE 0x7D

// Function to perform byte stuffing
void byte_stuffing(unsigned char *input, unsigned char *output, int length) {
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (input[i] == FLAG_BYTE) {
            output[j++] = ESCAPE_BYTE;  // 0x7D
            output[j++] = 0x5E;         // Stuffed 0x7E as 0x7D 0x5E
        } else if (input[i] == ESCAPE_BYTE) {
            output[j++] = ESCAPE_BYTE;  // 0x7D
            output[j++] = 0x5D;         // Stuffed 0x7D as 0x7D 0x5D
        } else {
            output[j++] = input[i];     // Normal data, no stuffing needed
        }
    }
    output[j] = '\0';  // Null-terminate the output
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    unsigned char buffer[MAX] = {0};
    unsigned char stuffed_data[MAX] = {0};

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

    // Read the original data sent by client
    int bytes_read = read(new_socket, buffer, MAX);
    printf("Original Data received: ");
    for (int i = 0; i < bytes_read; i++) {
        printf("0x%02X ", buffer[i]);  // Display as Hexadecimal
    }
    printf("\n");

    // Perform byte stuffing
    byte_stuffing(buffer, stuffed_data, bytes_read);

    printf("Stuffed Data to send: ");
    for (int i = 0; stuffed_data[i] != 0; i++) {
        printf("0x%02X ", stuffed_data[i]);  // Display as Hexadecimal
    }
    printf("\n");

    // Send stuffed data back to client
    send(new_socket, stuffed_data, strlen((const char *)stuffed_data), 0);

    close(new_socket);
    return 0;
}
