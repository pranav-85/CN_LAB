#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ifaddrs.h>


#define PORT 8080


void getIPAddress(char* ip_buffer) {
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &addr->sin_addr, ip_buffer, INET_ADDRSTRLEN);
            break;
        }
    }
    freeifaddrs(ifaddr);
}


int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char ip_address[INET_ADDRSTRLEN];
    getIPAddress(ip_address);


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        return -1;
    }


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }


    send(sock, ip_address, strlen(ip_address), 0);


    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    printf("Server Response: %s\n", buffer);


    close(sock);
    return 0;
}
