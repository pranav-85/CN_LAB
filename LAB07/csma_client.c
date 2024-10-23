#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include<sys/time.h>
#include<math.h>

#define PORT 8090
#define MAX 100
#define TIMEOUT 5
#define K_MAX 5

int check_timeout(struct timeval start) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    double elapsed = (current_time.tv_sec - start.tv_sec) +
                     (current_time.tv_usec - start.tv_usec) / 1000000.0;
    return (elapsed >= TIMEOUT);
}

int power(int a, int b){
    int retval = 1;

    for(int i=0;i<b;i++){
        retval *= a;
    }

    return retval;
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error!");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid IPv4 address");
        exit(EXIT_FAILURE);
    }

    int val;
    while ((val = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        perror("Connection failed!");
        int sleep_time = (int)(rand() * 100);
        if(sleep_time < 0){
            sleep_time *= -1;
        }
        printf("Back-off time: %d\n", sleep_time);
        sleep(sleep_time);
    }

    printf("\n%d\n", val);
    int size = 3;

    int i=0;
    int k=0;

    while(i < size){
        int acknowledged = 0;

        while (!acknowledged) {
            // Send packet to server
            char buffer[MAX] = {0};
            sprintf(buffer, "%d", i);
            send(sock, buffer, strlen(buffer), 0);
            printf("Packet %d sent.\n", i);

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
                    if (sscanf(buffer, "ACK %d", &ack_no) == 1 && ack_no == i) {
                        printf("Acknowledgment received for Packet %d.\n", i);
                        acknowledged = 1;
                        i++;
                        k=0;
                        break;
                    }
                }

                // Timeout check
                if (check_timeout(start_time)) {
                    k++;
                    if(k > K_MAX){
                        i++;
                        k = 0;
                        break;
                    }
                    else{
                        printf("Timeout occurred. Retransmitting Packet %d.\n", i);
                        int val = (int)power(2, k);
                        int sleep_time = ((int)rand() + RAND_MAX) % ((int)(val) - 1) * (TIMEOUT);
                        if(sleep_time < 0){
                            sleep_time *= -1;
                        }
                        printf("Back-off time: %d\n", sleep_time);
                        sleep(sleep_time);
                        break;
                    }
                }
            }
        }
    }
    close(sock);
    return 0;
}