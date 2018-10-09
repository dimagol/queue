//
// Created by dima on 08/10/18.
//

#ifndef TCP_SHMAFKA_MSGFROMCLIENT_H
#define TCP_SHMAFKA_MSGFROMCLIENT_H


#include "../../Buff/ThreadSafeBufferList.h"

class MsgFromClient {
public:
    MsgFromClient(BuffersList *bufferList, uint32_t fd) : bufferList(bufferList), fd(fd) {}

    BuffersList *getBufferList() {
        return bufferList;
    }

    uint32_t getFd() {
        return fd;
    }

private:
    uint32_t fd;
    BuffersList* bufferList;

};


#endif //TCP_SHMAFKA_MSGFROMCLIENT_H
