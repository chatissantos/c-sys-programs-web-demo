//
// Created by Chatis Santos on 2/18/23.
//

#ifndef SANTOS2_CLOCK_H
#define SANTOS2_CLOCK_H

typedef struct {
    int sec;
    int nano;
} Clock;

#define CLOCK_MEMORY_KEY 0x1236412
#define CLOCK_NANO_INCREMENT 800

#endif //SANTOS2_CLOCK_H
