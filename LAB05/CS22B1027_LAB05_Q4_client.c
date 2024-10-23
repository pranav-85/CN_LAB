// Client-side Code for Selective Repeat ARQ Protocol
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdbool.h>

#define PORT 8080
#define MAX 1024
#define TIMEOUT 3  // Timeout in seconds

int total_packets;
int window_size;
bool acknowledged[MAX];

// Function to check if the timeout has occurred
int check_timeout(struct timeval start) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    double elapsed = (current_time.tv_sec - start.tv_sec) +
                     (current_time.tv_usec - start.tv_usec) / 1000000.0;
    return (elapsed >= TIMEOUT);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

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

    printf("Enter the total number of packets to be sent: ");
    scanf("%d", &total_packets);

    printf("Enter the window size: ");
    scanf("%d", &window_size);

    memset(acknowledged, 0, sizeof(acknowledged));

    int base = 0, next_seq_num = 0;
    struct timeval start_time[MAX];  // Store the time each packet was sent

    while (base < total_packets) {
        // Send packets within the window
        while (next_seq_num < base + window_size && next_seq_num < total_packets) {
            if (!acknowledged[next_seq_num]) {
                // Send packet to server
                char buffer[MAX];
                sprintf(buffer, "PACKET %d", next_seq_num);
                send(sock, buffer, strlen(buffer), 0);
                printf("Packet %d sent.\n", next_seq_num);
                gettimeofday(&start_time[next_seq_num], NULL);  // Record the send time
            }
            next_seq_num++;
        }

        // Wait for acknowledgment
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(sock + 1, &read_fds, NULL, NULL, &timeout);

        if (activity > 0 && FD_ISSET(sock, &read_fds)) {
            char buffer[MAX];
            int bytes_read = read(sock, buffer, MAX);
            buffer[bytes_read] = '\0';

            int ack_no;
            if (sscanf(buffer, "ACK %d", &ack_no) == 1) {
                printf("Acknowledgment received for Packet %d.\n", ack_no);
                acknowledged[ack_no] = true;

                // Move the base if all packets up to the current base have been acknowledged
                while (acknowledged[base] && base < total_packets) {
                    base++;
                }
            }
        }

        // Check for timeout and retransmit if necessary
        for (int i = base; i < next_seq_num; i++) {
            if (!acknowledged[i] && check_timeout(start_time[i])) {
                // Retransmit the unacknowledged packet
                char buffer[MAX];
                sprintf(buffer, "PACKET %d", i);
                send(sock, buffer, strlen(buffer), 0);
                printf("Timeout occurred. Retransmitting Packet %d.\n", i);
                gettimeofday(&start_time[i], NULL);  // Reset the send time
            }
        }
    }

    close(sock);
    return 0;
}
