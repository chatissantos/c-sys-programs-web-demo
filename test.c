//
// Created by Chatis Santos on 3/18/23.
//
#include <stdio.h>
#include <stdlib.h>

int main () {
    printf("HOST_NAME : %s\n", getenv("HOST_NAME"));
    printf("PORT : %s\n", getenv("PORT"));
    return(0);
}
