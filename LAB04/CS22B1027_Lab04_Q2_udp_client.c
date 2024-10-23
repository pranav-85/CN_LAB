#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main() {
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);
    char message[] = "Hello, my name is Pranav";
    char buffer[1024] = {0};

    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_socket == -1) {
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error in sending message!");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &addrlen) < 0) {
        perror("Error in receiving message");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buffer);

    close(client_socket);
    return 0;
}
