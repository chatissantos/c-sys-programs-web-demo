//
// Created by Chatis Santos on 3/18/23.
//

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include "src/type/message.h"

int main() {
    printf("Oss started will wait to receive message\n");
    int qid = msgget(SERVER_TO_OSS_MESSAGE_KEY, 0644);
    Message msg;
    if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), 1,
               MSG_NOERROR | IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("No message available for msgrcv()\n");
    }

    printf("Oss got message from server: %s\n", msg.mtext);
    char * token = strtok(msg.mtext, ",");
    long arguments[3];
    int i = 0;
    char *p;
    while( token != NULL ) {
        arguments[i] = strtol(token, &p, 10);
        token = strtok(NULL, " ");
        i++;
    }
    long n = arguments[0];
    long s = arguments[1];
    long t = arguments[2];

    printf("Oss received message n: %ld, s: %ld, t: %ld\n", n, s, t);
    return 0;
}
