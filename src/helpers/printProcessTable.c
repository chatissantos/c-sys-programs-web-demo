//
// Created by Chatis Santos on 2/18/23.
//

#include <stdio.h>
#include "../../headers/logOss.h"
#include "../../headers/printProcessTable.h"

void printSeparator(int in) {
    int i;
    for(i = 0; i < 79; i++) {
        logOss(in, "%s", "-");
    }
    logOss(in, "\n");
}
void printHeader(int in) {
    printSeparator(in);
    logOss(in, "%5s | %15s | %15s | %15s | %15s |\n", "Entry", "Occupied", "PID", "StartS", "StartN");
    printSeparator(in);
}
void printTitle(int in) {
    logOss(in, "--- Process Table ---\n");
}
void printProcessTable(PCB processTable[], int size, int in) {
    printTitle(in);
    printHeader(in);
    int i;
    for(i = 0; i < size; i++) {
        logOss(in, "%5d | %15d | %15d | %15d | %15d |\n", i, processTable[i].occupied, processTable[i].pid, processTable[i].startSec, processTable[i].startNano);
    }
}