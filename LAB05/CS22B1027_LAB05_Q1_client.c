// Client-side Code for Byte De-stuffing using Hexadecimal
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024
#define FLAG_BYTE 0x7E
#define ESCAPE_BYTE 0x7D

// Function to perform byte de-stuffing
void byte_destuffing(unsigned char *input, unsigned char *output, int length) {
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (input[i] == ESCAPE_BYTE) {
            if (input[i + 1] == 0x5E) {
                output[j++] = FLAG_BYTE;  // De-stuffed 0x7D 0x5E back to 0x7E
            } else if (input[i + 1] == 0x5D) {
                output[j++] = ESCAPE_BYTE;  // De-stuffed 0x7D 0x5D back to 0x7D
            }
            i++;  // Skip the next byte (already processed)
        } else {
            output[j++] = input[i];  // Normal data, no de-stuffing needed
        }
    }
    output[j] = '\0';  // Null-terminate the output
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    unsigned char buffer[MAX] = {0};
    unsigned char destuffed_data[MAX] = {0};

    // Original data to be sent (in hexadecimal)
    unsigned char original_data[] = {0x45, 0x7E, 0x56, 0x7D, 0x78};  // Example data
    int original_data_len = sizeof(original_data) / sizeof(original_data[0]);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Send original data
    send(sock, original_data, original_data_len, 0);
    printf("Original Data sent: ");
    for (int i = 0; i < original_data_len; i++) {
        printf("0x%02X ", original_data[i]);  // Display as Hexadecimal
    }
    printf("\n");

    // Receive stuffed data from the server
    int bytes_read = read(sock, buffer, MAX);
    printf("Stuffed Data received: ");
    for (int i = 0; i < bytes_read; i++) {
        printf("0x%02X ", buffer[i]);  // Display as Hexadecimal
    }
    printf("\n");

    // Perform byte de-stuffing
    byte_destuffing(buffer, destuffed_data, bytes_read);

    printf("De-stuffed Data: ");
    for (int i = 0; i < original_data_len; i++) {
        printf("0x%02X ", destuffed_data[i]);  // Display as Hexadecimal
    }
    printf("\n");

    close(sock);
    return 0;
}
