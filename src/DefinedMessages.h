//
// Created by dima on 13/10/17.
//

#ifndef TCP_SHMAFKA_DEFINEDMESSAGES_H
#define TCP_SHMAFKA_DEFINEDMESSAGES_H


#include "Buff/BufferPool.h"

class DefinedMessages {
public:
    static SocketProtoBuffer * hello_msg;
    static SocketProtoBuffer * goodbye_msg;
};


#endif //TCP_SHMAFKA_DEFINEDMESSAGES_H
