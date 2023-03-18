#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/msg.h>
#include "../../headers/forkAndExecWorker.h"
#include "../type/message.h"

int forkAndExecWorker (int sec, int nano) {
    int pid = fork();
    if (pid == 0) {
        char* argument_list[] = {"./worker", NULL};
        int qid = msgget(OSS_TO_WORKER_MESSAGE_KEY, IPC_CREAT | 0666);
        Message msg;
        msg.mtype = 1;
        time_t t;

        time(&t);
        snprintf(msg.mtext, sizeof(msg.mtext), "%d|%d", sec, nano);

        if (msgsnd(qid, (void *) &msg, sizeof(msg.mtext),
                   IPC_NOWAIT) == -1) {
            perror("msgsnd error");
            exit(EXIT_FAILURE);
        }
        execv(argument_list[0],  argument_list);
        printf("execv failed to execute correctly\n");
        exit(1);
    }
    return pid;
}