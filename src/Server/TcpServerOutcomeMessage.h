//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
#define TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H


#include <cstdint>
#include <unordered_set>
#include "../Buff/BufferPool.h"
#include "../Logging/TSLogger.h"

using namespace std;
class TcpServerOutcomeMessage {
public:
    enum Type{NORMAL,DISCONNECT};
    TcpServerOutcomeMessage(SocketProtoBuffer *buffer, const unordered_set<uint32_t> &sendToSet) :
            buffer(buffer),
            sendToSet(sendToSet) {}

    TcpServerOutcomeMessage(SocketProtoBuffer *buffer, uint32_t sendTo) :
            buffer(buffer)
            {
                sendToSet.insert(sendTo);
            }

    TcpServerOutcomeMessage(uint32_t sendTo,Type type):
            type(type){
        if(type != DISCONNECT){
            LOG_ERROR("unsupported type");
        } else{
            sendToSet.insert(sendTo);
            buffer = nullptr;
        }
    }
    SocketProtoBuffer *getBuffer() const {
        return buffer;
    }

    const unordered_set<uint32_t> &getSendToSet() const {
        return sendToSet;
    }

    Type getType() const {
        return type;
    }
private:
    Type type = NORMAL;
    SocketProtoBuffer * buffer;
    unordered_set<uint32_t> sendToSet;


};


#endif //TCP_SHMAFKA_TCPSERVEROUTCOMEMESSAGE_H
