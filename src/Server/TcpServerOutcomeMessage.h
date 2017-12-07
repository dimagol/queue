//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
#define TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H


#include <cstdint>
#include <bits/unordered_set.h>
#include "../Buff/BufferPool.h"
using namespace std;
class TcpServerOutcomeMessage {

private:
    SocketProtoBuffer * buffer;
    const unordered_set<uint32_t> sendToSet;


};


#endif //TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
