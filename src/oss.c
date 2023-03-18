#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/signal.h>
#include "type/clock.h"
#include "../headers/printProcessTable.h"
#include "../headers/parseArguments.h"
#include "../headers/checkArguments.h"
#include "../headers/forkAndExecWorker.h"
#include "../headers/signal.h"
#include "../headers/logOss.h"
#include <sys/types.h>
#include <sys/wait.h>

PCB * processTable;
int totalWorkerCount;
int shm_id;

static void timeoutHandler(int s) {
    logOss("Program timed out. Exiting.......\n");

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
    srand(time(NULL));
    int randomSeconds;
    Arguments arguments = parseArguments(argc, argv);
    checkArguments(arguments);

    totalWorkerCount = arguments.totalWorkerCount;

    if (setupinterrupt(timeoutHandler) == -1) {
        logOss("Failed to set up handler for SIGPROF\n");
        exit(1);
    }
    if (setupitimer() == -1) {
        logOss("Failed to set up the ITIMER_PROF interval timer\n");
        exit(1);
    }


    processTable = (PCB *)malloc(sizeof(PCB) *totalWorkerCount);

    shm_id = shmget(CLOCK_MEMORY_KEY, sizeof(Clock), IPC_CREAT | 0644);

    if (shm_id == -1) {
        logOss("shmget failed in oss \n");
        exit(1);
    }

    Clock * clock = (Clock *) shmat(shm_id, NULL, 0);
    if (clock == (void *) -1) {
        logOss("shmat failed in oss\n");
        exit(1);
    }

    clock->nano = 0;
    clock->sec = 0;

    int activeWorkerCount = 0;
    int nextWorker = 0;

    int status;
    int pid = 0;
    int i;

    logOss("\n\n\t\t= OSS STARTED %ld =\n\n", time(NULL));

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
            printProcessTable(processTable, totalWorkerCount);
        }
        if (nextWorker < totalWorkerCount) {
            if (activeWorkerCount < arguments.maximumConcurrentProcesses) {
                randomSeconds = (rand() % arguments.maximumChildRunTimeInSeconds) + 1;
                processTable[nextWorker].pid = forkAndExecWorker(clock->sec + randomSeconds, clock->nano);
                processTable[nextWorker].occupied = 1;
                processTable[nextWorker].startSec = clock->sec;
                processTable[nextWorker].startNano = clock->nano;

                nextWorker += 1;
                activeWorkerCount += 1;
                printProcessTable(processTable, totalWorkerCount);
            }
        }
        if ((pid = waitpid(-1, &status, WNOHANG)) == -1) {
            break;
        }
    }

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
