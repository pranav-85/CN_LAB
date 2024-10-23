#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>

#define PORT 8001
#define MAX 100

int main(){

    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;
    char buffer[MAX];   
    char* message = "Client";

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr) < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if(connect(client_sock, (struct sockaddr* )&client_address, sizeof(client_address)) < 0){
        perror("Error while connecting...");
        exit(EXIT_FAILURE);
    }

    if(send(client_sock, message, sizeof(message), 0) < 0){
        perror("Error while requesting...");
        exit(EXIT_FAILURE);
    }

    if(recv(client_sock, buffer, sizeof(buffer), 0) < 0){
        perror("Error while receiving data..");
        exit(EXIT_FAILURE);
    }
    
    printf("%s\n\n", buffer);
    close(client_sock);
    return 0;
}