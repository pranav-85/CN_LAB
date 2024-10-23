#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 8001
#define MAX 100

int main(){

    struct sockaddr_in server_address;
    socklen_t addrlen = sizeof(server_address);
    char buffer[MAX];
    char* message = "Hello, My name is Pranav!!";

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(server_sock == -1){
        perror("Error in creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    // server_address.sin_addr.s_addr = INADDR_ANY;

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if(bind(server_sock, (struct sockaddr* )&server_address, addrlen) < 0){
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    if(listen(server_sock, 1) < 0){
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    int client_sock = accept(server_sock, (struct sockaddr *)&server_address, &addrlen);

    if(read(client_sock, buffer, sizeof(buffer)) < 0){
        perror("Error in reading");
        exit(EXIT_FAILURE);
    }

    send(client_sock, message, strlen(message), 0);

    printf("%s\n\n", buffer);
    close(server_sock);
}