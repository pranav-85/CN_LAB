#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

#define PORT 8090
#define MAX 100
#define COLLISION_RATE 0.9

int Collision(){
    float random_value = (float)rand() / RAND_MAX;

    if (random_value < COLLISION_RATE) {
        return 1; 
    } else {
        return 0;
    }
}


int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    char buffer[MAX] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error!");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Binding error!");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1) < 0)
    {
        perror("Error encountered while listening!");
        exit(EXIT_FAILURE);
    }

    int new_sock;
    if ((new_sock = accept(sock, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("Error while accepting!");
        exit(EXIT_FAILURE);
    }

    printf("Connection established!\n\n");

    
    while (1) {
        int bytes_read = read(new_sock, buffer, MAX);
        if (bytes_read <= 0) {
            printf("Connection closed or error.\n");
            break;
        }

        int packet_no;
        sscanf(buffer, "%d", &packet_no);

        // Simulate packet corruption
        if (Collision()) {
            printf("Collision detected! No ACK is sent\n");
        } else {

            int user_ack=1;
            if (user_ack == 1) {
                // Send acknowledgment
                char ack_message[MAX];
                sprintf(ack_message, "ACK %d", packet_no);
                send(new_sock, ack_message, strlen(ack_message), 0);
                printf("Acknowledgment sent for %d.\n",packet_no);
            } 
        }
    }

    close(sock);
    close(new_sock);
    return 0;
}