//
// Created by Chatis Santos on 2/28/23.
//

#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include "../../headers/signal.h"

int setupinterrupt(void (*f)(int)) {
    struct sigaction act;
    act.sa_handler = (*f);
    act.sa_flags = 0;
    return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}
int setupitimer() {
    struct itimerval value;
    value.it_interval.tv_sec = 60;
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_PROF, &value, NULL));
}
