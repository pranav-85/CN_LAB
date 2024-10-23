// Client-side Code for Go-Back-N ARQ Protocol (with Timeout and Retransmission)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define MAX 1024
#define TIMEOUT 2  // Timeout in seconds

int total_packets;
int window_size;
char buffer[MAX] = {0};

// Function to check timeout
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

    int base = 0, next_seq_num = 0;
    struct timeval start_time;

    while (base < total_packets) {
        // Send packets within the window
        while (next_seq_num < base + window_size && next_seq_num < total_packets) {
            // Send packet to server
            sprintf(buffer, "PACKET %d", next_seq_num);
            send(sock, buffer, strlen(buffer), 0);
            printf("Packet %d sent.\n", next_seq_num);
            next_seq_num++;
        }

        // Wait for acknowledgment
        gettimeofday(&start_time, NULL);
        int acknowledged = 0;

        while (!acknowledged) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(sock, &read_fds);

            struct timeval timeout;
            timeout.tv_sec = TIMEOUT;
            timeout.tv_usec = 0;

            int activity = select(sock + 1, &read_fds, NULL, NULL, &timeout);

            if (activity > 0 && FD_ISSET(sock, &read_fds)) {
                int bytes_read = read(sock, buffer, MAX);
                buffer[bytes_read] = '\0';

                int ack_no;
                if (sscanf(buffer, "ACK %d", &ack_no) == 1) {
                    printf("Acknowledgment received for Packet %d.\n", ack_no);
                    base = ack_no + 1;  // Move the window
                    acknowledged = 1;
                }
            }

            // Check for timeout
            if (check_timeout(start_time)) {
                printf("Timeout occurred. Retransmitting from Packet %d.\n", base);
                next_seq_num = base;  // Retransmit starting from base
                break;  // Retransmit packets within the window
            }
        }
    }

    close(sock);
    return 0;
}
