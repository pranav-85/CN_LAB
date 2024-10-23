#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8090
#define MAX 100
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

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed!");
        exit(EXIT_FAILURE);
    }

    char file_name[MAX];
    char buffer[MAX];

    printf("Enter file name: ");
    scanf("%s", file_name);

    // Requesting file from server by sending file name.
    send(sock, file_name, sizeof(file_name), 0);

    read(sock, buffer, MAX);
    printf("%s\n", buffer);

    if (strcmp(buffer, "File found!") == 0)
    {
        printf("Receiving the file....\n");

        FILE *fptr;
        fptr = fopen(file_name, "w");
        while (1)
        {
            read(sock, buffer, MAX);
            if (!strcmp(buffer, "EOF"))
            {
                break;
            }
            
            printf("%s\n", buffer);
            fputs(buffer, fptr);
        }

        printf("File received successfully!\n");

        fclose(fptr);

        printf("\n\nDisplaying the file...\n\n");


        fptr = fopen(file_name, "r");
        char data[MAX];
    
        while (fgets(data, MAX, fptr))
        {
            printf("%s\n", data);
        }
    }

    else
    {
        printf("File not found!\n");
    }

    close(sock);
    return 0;
}