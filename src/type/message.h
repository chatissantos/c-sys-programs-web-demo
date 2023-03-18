//
// Created by Chatis Santos on 3/14/23.
//

#ifndef SANTOS_3_MESSAGE_H
#define SANTOS_3_MESSAGE_H
#define OSS_TO_WORKER_MESSAGE_KEY 0x4678908
#define SERVER_TO_OSS_MESSAGE_KEY 0x4678908
typedef struct {
    long mtype;
    char mtext[80];
} Message;
#endif //SANTOS_3_MESSAGE_H
