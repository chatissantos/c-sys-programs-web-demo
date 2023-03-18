#include <stdlib.h>
#include <stdio.h>
#include "../../headers/checkArguments.h"
#include "../../headers/handleHelpMessage.h"

void checkArguments(Arguments  arguments) {
    if (handleHelpMessage(arguments.helpFlag) == 0) {
        exit(0);
    }
    if (arguments.totalWorkerCount == 0) {
        printf("total number of processes not supplied. supply with `-n :value`\n");
        exit(1);
    }

    if (arguments.maximumConcurrentProcesses == 0) {
        printf("max number of concurrent children not supplied. supply with `-s :value`\n");
        exit(1);
    }

    if (arguments.maximumChildRunTimeInSeconds == 0) {
        printf("number of iteration not supplied for worker. supply with `-t :value`\n");
        exit(1);
    }
}