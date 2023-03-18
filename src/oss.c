#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/signal.h>
#include "type/clock.h"
#include "../headers/printProcessTable.h"
//#include "../headers/parseArguments.h"
#include "../headers/checkArguments.h"
#include "../headers/forkAndExecWorker.h"
#include "../headers/signal.h"
#include "../headers/logOss.h"
#include "./type/message.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <printf.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

PCB * processTable;
long totalWorkerCount;
int shm_id;

static void timeoutHandler(int s) {
//    logOss("Program timed out. Exiting.......\n");

    int i;
    for(i = 0; i < totalWorkerCount; i++) {
        if (processTable[i].occupied > 0) {
            kill(processTable[i].pid, SIGKILL);
        }
    }

    shmctl(shm_id, IPC_RMID, NULL);

    exit(0);
}
void incrementClock(Clock *clock)  {
    clock->nano += CLOCK_NANO_INCREMENT;
    if (clock->nano >= 1000000000) {
        clock->nano = 0;
        clock->sec += 1;
    }
}

int main(int argc, char *argv[]) {
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
        token = strtok(NULL, ",");
        i++;
    }
    long n = arguments[0];
    long s = arguments[1];
    long t = arguments[2];

    printf("Oss received message n: %ld, s: %ld, t: %ld\n", n, s, t);

    int shmidServer = shmget(getpid(), sizeof(Client), 0644);
    if (shmidServer == -1) {
        printf("shmget failed in OSS 1\n");
        exit(1);
    }

    Client * client = (Client * ) shmat(shmidServer, NULL, 0);
    if (client == (void *) -1) {
        printf("shmat failed in OSS 1\n");
        exit(1);
    }

    printf("IN FROM OSS: %d\n", client->in);
    send(client->in, "SENT TO CLIENT FROM OSS", 256, 0);


    srand(time(NULL));
    int randomSeconds;

    totalWorkerCount = n;
    long maximumConcurrentProcesses = s;
    long maximumChildRunTimeInSeconds = t;

    if (setupinterrupt(timeoutHandler) == -1) {
        logOss(client->in, "Failed to set up handler for SIGPROF\n");
        exit(1);
    }
    if (setupitimer() == -1) {
        logOss(client->in, "Failed to set up the ITIMER_PROF interval timer\n");
        exit(1);
    }


    processTable = (PCB *)malloc(sizeof(PCB) *totalWorkerCount);

    shm_id = shmget(CLOCK_MEMORY_KEY, sizeof(Clock), IPC_CREAT | 0644);

    if (shm_id == -1) {
        logOss(client->in, "shmget failed in oss \n");
        exit(1);
    }

    Clock * clock = (Clock *) shmat(shm_id, NULL, 0);
    if (clock == (void *) -1) {
        logOss(client->in, "shmat failed in oss\n");
        exit(1);
    }

    clock->nano = 0;
    clock->sec = 0;

    int activeWorkerCount = 0;
    int nextWorker = 0;

    int status;
    int pid = 0;

    logOss(client->in, "\n\n\t\t= OSS STARTED %ld =\n\n", time(NULL));

    for(;;) {
        incrementClock(clock);
        if (pid > 0) {
            activeWorkerCount--;
            for (i = 0; i < totalWorkerCount; i++) {
                if (processTable[i].pid == pid) {
                    processTable[i].occupied = 0;
                    break;
                }
            }
            printProcessTable(processTable, totalWorkerCount, client->in);
        }
        if (nextWorker < totalWorkerCount) {
            if (activeWorkerCount < maximumConcurrentProcesses) {
                randomSeconds = (rand() % maximumChildRunTimeInSeconds) + 1;
                processTable[nextWorker].pid = forkAndExecWorker(clock->sec + randomSeconds, clock->nano);
                processTable[nextWorker].occupied = 1;
                processTable[nextWorker].startSec = clock->sec;
                processTable[nextWorker].startNano = clock->nano;

                nextWorker += 1;
                activeWorkerCount += 1;
                printProcessTable(processTable, totalWorkerCount, client->in);
            }
        }
        if ((pid = waitpid(-1, &status, WNOHANG)) == -1) {
            break;
        }
    }

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
