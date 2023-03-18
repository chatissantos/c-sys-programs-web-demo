#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include "type/clock.h"
#include "type/message.h"
#include <sys/msg.h>
#include <errno.h>

//
// Created by Chatis Santos on 2/16/23.
//
int main() {
    int qid = msgget(OSS_TO_WORKER_MESSAGE_KEY, 0644);
    Message msg;
    if (msgrcv(qid, (void *) &msg, sizeof(msg.mtext), 1,
               MSG_NOERROR | IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("No message available for msgrcv()\n");
    };


    /*
     * Get sec and nano by splitting the message string
     */
    char * token = strtok(msg.mtext, "|");
    long times[2];
    int i = 0;
    char *p;
    while( token != NULL ) {
        times[i] = strtol(token, &p, 10);
        token = strtok(NULL, " ");
        i++;
    }
    long timeOutSec = times[0];
    long timeOutNano = times[1];

    int shmid = shmget(CLOCK_MEMORY_KEY, sizeof(Clock), 0644);
    if (shmid == -1) {
        printf("shmget failed in worker\n");
        exit(1);
    }

    Clock * clock = (Clock *) shmat(shmid, NULL, 0);
    if (clock == (void *) -1) {
        printf("shmat failed in worker\n");
        exit(1);
    }

    int pid = getpid();
    int ppid = getppid();
    printf("WORKER PID:%d PPID:%d SysClockS: %d SysclockNano: %d TermTimeS: %ld TermTimeNano: %ld\n", pid, ppid, clock->sec, clock->nano, timeOutSec, timeOutNano );
    printf("--Just Starting\n");

    int currentSecond = clock->sec;
    int secondsPassed = 0;
    while(clock->sec < timeOutSec || clock->nano < timeOutNano) {
        if (clock->sec != currentSecond) {
            currentSecond = clock->sec;
            secondsPassed += 1;
            printf("WORKER PID:%d PPID:%d SysClockS: %d SysclockNano: %d TermTimeS: %ld TermTimeNano: %ld\n", pid, ppid, clock->sec, clock->nano, timeOutSec, timeOutNano );
            printf("--%d seconds have passed since starting\n", secondsPassed);
        }
    }

    shmdt(clock);

    return 0;
}
