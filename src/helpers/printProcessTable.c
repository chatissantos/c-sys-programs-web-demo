//
// Created by Chatis Santos on 2/18/23.
//

#include <stdio.h>
#include "../../headers/logOss.h"
#include "../../headers/printProcessTable.h"

void printSeparator() {
    int i;
    for(i = 0; i < 79; i++) {
        logOss("%s", "-");
    }
    logOss("\n");
}
void printHeader() {
    printSeparator();
    logOss("%5s | %15s | %15s | %15s | %15s |\n", "Entry", "Occupied", "PID", "StartS", "StartN");
    printSeparator();
}
void printTitle() {
    logOss("--- Process Table ---\n");
}
void printProcessTable(PCB processTable[], int size) {
    printTitle();
    printHeader();
    int i;
    for(i = 0; i < size; i++) {
        logOss("%5d | %15d | %15d | %15d | %15d |\n", i, processTable[i].occupied, processTable[i].pid, processTable[i].startSec, processTable[i].startNano);
    }
}