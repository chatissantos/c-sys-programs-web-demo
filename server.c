//
// Created by Chatis Santos on 3/18/23.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include "src/type/message.h"
#include <sys/wait.h>
#include <sys/shm.h>

int main() {
    int fd = 0;
    char buff[80];

    //Setup Buffer Array
    memset(buff, '0',sizeof(buff));

    //Create Socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0) {
        perror("Client Error: Socket not created successfully");
        return 0;
    }

    //Structure to store details
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));

    //Initialize
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = htonl(INADDR_ANY);


    bind(fd, (struct sockaddr*)&server, sizeof(server));

    int in;

    listen(fd, 10);
    while((in = accept(fd, (struct sockaddr*)NULL, NULL))) {
        int childpid,n;
        if ( (childpid = fork ()) == 0 ) {

            printf ("\nOne Client Connected !! ");

            //close listening socket
            close (fd);

            //Clear Zeroes
            bzero(buff,80);

            while ( (n = recv(in, buff, 256,0)) > 0) {

                printf("Server Received from client %d: %s\n",getpid(),buff);

                int pid = fork();
                if (pid == 0) {
                    printf("Lunched a child process for OSS\n");
                    char* argument_list[] = {"./oss", NULL};
                    int qid = msgget(SERVER_TO_OSS_MESSAGE_KEY, IPC_CREAT | 0666);
                    Message msg;
                    msg.mtype = 1;
                    time_t t;

                    time(&t);
                    snprintf(msg.mtext, sizeof(msg.mtext), "%s", buff);

                    if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext),
                               IPC_NOWAIT) == -1) {
                        perror("msgsnd error");
                        exit(EXIT_FAILURE);
                    }


                    int shm_id = shmget(getpid(), sizeof(Client), IPC_CREAT | 0644);

                    if (shm_id == -1) {
                        printf("shmget failed in oss \n");
                        exit(1);
                    }


                    Client * client = (Client *) shmat(shm_id, NULL, 0);
                    if (client == (void *) -1) {
                        printf("shmat failed in oss\n");
                        exit(1);
                    }
                    client->in = in;
                    printf("IN FROM SERVER: %d\n", in);

                    execv(argument_list[0],  argument_list);
                    printf("execv failed to execute correctly\n");
                    exit(1);
                }
                while(wait(NULL) != -1 || errno != ECHILD);
                printf("OSS HAS RETURNED\n");

                //Response to client
                send(in, "<html><body>alibaba</body></html>", 256, 0);

                bzero(buff,80);

            }
            close(in);
            exit(0);
        }
    }
}