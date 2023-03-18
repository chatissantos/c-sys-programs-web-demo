//
// Created by Chatis Santos on 3/18/23.
//

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char * PORT = getenv("PORT");
    char * HOST_NAME = getenv("HOST_NAME");
    char buff[1024];

    if(PORT == NULL) {
        PORT = (char *) malloc(sizeof(char) * 20);
        printf("ENTER PORT: ");
        scanf("%s", PORT);
    }
    if(HOST_NAME == NULL) {
        HOST_NAME = (char *) malloc(sizeof(char) * 255);
        printf("ENTER HOST_NAME: ");
        scanf("%s", HOST_NAME);
    }

    //Setup Buffer Array
    memset(buff, '0',sizeof(buff));

    //Create Socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0) {
        perror("Client Error: Socket not created succesfully");
        return 0;
    }

    //Structure to store details
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));

    //Initialize
    server.sin_family = AF_INET;
    server.sin_port =  htons(atoi(PORT));

    int in = inet_pton(AF_INET, HOST_NAME, &server.sin_addr);
    if(in<0) {
        perror("Client Error: IP not initialized succesfully");
        return 0;
    }

    //Connect to given server
    in = connect(fd, (struct sockaddr *)&server, sizeof(server));
    if(in<0) {
        printf("Connection to host %s on port %s has failed.\n", HOST_NAME, PORT);
        perror("Error");
        return 0;
    }

    while(1) {
        printf("Please enter the message: ");
        bzero(buff,256);
        fgets(buff,255,stdin);

//        printf("\nSending to SERVER: %s ", buff);

        /* Send message to the server */
        in = send(fd,buff,strlen(buff),0);
        if (in < 0) {
            perror("\nClient Error: Writing to Server");
            return 0;
        }

        /* Now read server response */
        bzero(buff, 256);
        int n;
        while ( (in = recv(fd, buff, 256,0)) > 0) {
//        while ( (n = recv(in, buff, 256,0)) > 0) {
//            in = recv(fd,buff,255,0);
            if (in < 0) {
                perror("\nClient Error: Reading from Server");
                return 0;
            }
            printf("%s",buff);
        }
    }
    close(fd);
    return 0;
}
