#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../headers/parseArguments.h"

Arguments parseArguments(int argc, char *argv[]) {
    Arguments arguments;
    arguments.helpFlag = 0;
    arguments.totalWorkerCount = 0;
    arguments.maximumConcurrentProcesses = 0;
    arguments.maximumChildRunTimeInSeconds = 0;
//    logFileName = (char *)malloc(sizeof(char) * (20));
//    sprintf(logFileName, "%ld", time(NULL));
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "n:s:t:hf:")) != -1)
        switch (c) {
            case 'n':
                arguments.totalWorkerCount = atoi(optarg);
                break;
            case 's':
                arguments.maximumConcurrentProcesses = atoi(optarg);
                break;
            case 't':
                arguments.maximumChildRunTimeInSeconds = atoi(optarg);
                break;
            case 'h':
                arguments.helpFlag = 1;
                break;
//            case 'f':
//                logFileName = optarg;
//                break;
            case '?':
                if (optopt == 'n' || optopt == 's' || optopt == 't')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
            default:
                abort ();
        }

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    return arguments;
}
