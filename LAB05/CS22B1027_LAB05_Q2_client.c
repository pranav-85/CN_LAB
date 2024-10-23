// Client-side Code for Stop-and-Wait ARQ with Handling Retransmissions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define MAX 1024
#define TIMEOUT 10  // Timeout in seconds

int total_packets;
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

    int packet_no = 0;

    while (packet_no < total_packets) {
        int acknowledged = 0;

        while (!acknowledged) {
            // Send packet to server
            sprintf(buffer, "PACKET %d", packet_no);
            send(sock, buffer, strlen(buffer), 0);
            printf("Packet %d sent.\n", packet_no);

            // Wait for acknowledgment
            struct timeval start_time;
            gettimeofday(&start_time, NULL);

            while (1) {
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
                    if (sscanf(buffer, "ACK %d", &ack_no) == 1 && ack_no == packet_no) {
                        printf("Acknowledgment received for Packet %d.\n", packet_no);
                        acknowledged = 1;
                        packet_no++;
                        break;
                    }
                }

                // Timeout check
                if (check_timeout(start_time)) {
                    printf("Timeout occurred. Retransmitting Packet %d.\n", packet_no);
                    break;  // Retransmit the same packet
                }
            }
        }
    }

    close(sock);
    return 0;
}
