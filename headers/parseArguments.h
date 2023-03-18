#ifndef SANTOS1_PARSEOPTIONS_H
#define SANTOS1_PARSEOPTIONS_H

typedef struct {
    int helpFlag;
    int totalWorkerCount;
    int maximumConcurrentProcesses;
    int maximumChildRunTimeInSeconds;
} Arguments;

Arguments parseArguments(int argc, char *argv[]) ;

#endif //SANTOS1_PARSEOPTIONS_H
