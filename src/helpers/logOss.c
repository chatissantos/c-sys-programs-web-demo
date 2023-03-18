//
// Created by Chatis Santos on 3/14/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../../headers/logOss.h"

void logOss(char * string, ...) {
    va_list args;

    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    va_start(args, string);
    FILE *fp;
    fp = fopen("log", "a");
    vfprintf(fp, string, args);
    fclose(fp);
    va_end(args);
}
