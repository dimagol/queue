//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_TCPSERVERINCOMEMESSAGE_H
#define TCP_SHMAFKA_TCPSERVERINCOMEMESSAGE_H


#include "../Buff/BufferPool.h"
#include "TcpServerConnection.h"

class TcpServerIncomeMessage {
public:
    TcpServerIncomeMessage(SocketProtoBuffer *buffer,
                           const uint32_t id) : buffer(buffer),id(id) {}

    SocketProtoBuffer *getBuffer() {
        return buffer;
    }

    const uint32_t getId() const {
        return id;
    }

private:
    SocketProtoBuffer * buffer;
    const uint32_t id;

};


#endif //TCP_SHMAFKA_TCPSERVERINCOMEMESSAGE_H
