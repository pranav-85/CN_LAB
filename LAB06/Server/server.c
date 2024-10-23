#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8090
#define MAX 100

char files[MAX][MAX] = {"file1.txt", "file2.txt"};
int num_files = sizeof(files) / sizeof(files[0]);

int main()
{
    char data[MAX][MAX] = {"Hey, I am CS22B1027.\n", "Example file for CN LAB file transfer.\n", "Good Bye!!\n"};
    int num_msgs = sizeof(data) / sizeof(data[0]);

    FILE *fp = fopen("file1.txt", "w");

    printf("Creating file....\n");
    for(int i=0;i<num_msgs;i++){
        fputs(data[i] ,fp);
    }

    fclose(fp);
    printf("File successfully created!\n\n");
    int sock = 0;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);

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

    char file_name[MAX];

    read(new_sock, file_name, MAX);
    
    // printf("%s\n\n", file_name);
    int flag=1;
    for(int i=0; i < num_files; i++){
        if(!strcmp(files[i], file_name)){
            flag=1;
            break;
        }
    }

    if(!flag){
        printf("File not found!\n");
        char message[] = "File not found!";
        send(new_sock, message, sizeof(message), 0);
    }
    else{
        
        printf("File found!\n");
        char message[] = "File found!";
        send(new_sock, message, sizeof(message), 0);

        printf("Sending %s...\n", file_name);
        FILE *fptr;
        fptr = fopen(file_name, "r");

        char data[MAX];
        while(fgets(data, MAX, fptr)){
            printf("%s\n", data);
            send(new_sock, data, sizeof(data), 0);
        }

        char end_msg[] = "EOF";
        send(new_sock, end_msg, sizeof(end_msg), 0);

        printf("File transmission successful!!\n\n");
        
        fclose(fptr);
    }

    close(sock);
    close(new_sock);
    return 0;
}