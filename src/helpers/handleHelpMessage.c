//
// Created by Chatis Santos on 2/1/23.
//

#include <stdio.h>
#include "../../headers/handleHelpMessage.h"

int handleHelpMessage (int helpFlag) {
    if (helpFlag == 0) {
        return 1;
    }

    printf("=========================\n");
    printf("How to run the program? \n");
    printf("./oss -n :integer -t :integer -s :integer\n");
    printf("-n indicates the total number of child processes to launch\n");
    printf("-s indicates the maximum number of concurrent child processes\n");
    printf("-t indicates the second timeout of the worker\n");
    printf("=========================\n");
    return 0;
}