#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include <arpa/inet.h>
#define PORT 8000


int main(){

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char* message = "Message received successfully!";
    char buffer[1024] = {0};

    if(server_socket == -1){
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    if(bind(server_socket,(struct sockaddr*) &address, sizeof(address)) < 0){
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if(listen(server_socket, 1) < 0){
        perror("Error in listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    int client_socket = accept(server_socket, (struct sockaddr*)&address, &addrlen);

    if(client_socket < 0){
        perror("Connection accepting failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    read(client_socket, buffer, sizeof(buffer));
    printf("Client: %s\n", buffer);


    if(send(client_socket, message, strlen(message), 0) < 0){
        perror("Error in sending message!");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // close(client_socket);
    close(server_socket);

    return 0;
}
