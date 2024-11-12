#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>


#define PORT 8080


const char* classifyIP(int first_octet) {
    if (first_octet >= 0 && first_octet <= 127) return "Class A";
    if (first_octet >= 128 && first_octet <= 191) return "Class B";
    if (first_octet >= 192 && first_octet <= 223) return "Class C";
    if (first_octet >= 224 && first_octet <= 239) return "Class D";
    if (first_octet >= 240 && first_octet <= 255) return "Class E";
    return "Unknown Class";
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
   
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
   
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
   
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
   
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
   
    read(new_socket, buffer, 1024);
    int first_octet;
    sscanf(buffer, "%d", &first_octet);
    const char* classification = classifyIP(first_octet);


    char response[1024];
    snprintf(response, sizeof(response), "IP: %.15s, Classification: %.50s", buffer, classification);
    send(new_socket, response, strlen(response), 0);


    close(new_socket);
    close(server_fd);
    return 0;
}
