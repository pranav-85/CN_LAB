#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);  
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    char *message = "Message received successfully!";
    char buffer[1024] = {0};

    if (server_socket == -1) {
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addrlen) < 0) {
        perror("Error in receiving message");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Client: %s\n", buffer);

    if (sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&client_addr, client_addrlen) < 0) {
        perror("Error in sending message!");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    close(server_socket);
    return 0;
}
