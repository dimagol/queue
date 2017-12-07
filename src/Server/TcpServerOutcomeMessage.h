//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
#define TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H


#include <cstdint>
#include <unordered_set>
#include "../Buff/BufferPool.h"
using namespace std;
class TcpServerOutcomeMessage {

private:
    SocketProtoBuffer * buffer;
    const unordered_set<uint32_t> sendToSet;
public:
    SocketProtoBuffer *getBuffer() const {
        return buffer;
    }

    const unordered_set<uint32_t> &getSendToSet() const {
        return sendToSet;
    }


};


#endif //TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
