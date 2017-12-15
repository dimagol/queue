//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_PROCEEDEDMSG_H
#define TCP_SHMAFKA_PROCEEDEDMSG_H

#include <string>
#include "../Buff/BufferPool.h"
#include "../Server/TcpServerConnection.h"
#include "../Msg/MsgType.h"

using namespace std;

class ProceededEvent {

public:

    ProceededEvent(const string &channelName,
                 const MsgType type,
                 const uint32_t sender_id)
            : channelName(channelName),
              type(type),
              sender_id(sender_id){}

    ProceededEvent(const MsgType type,
                 const uint32_t sender_id)
            : channelName(""),
              type(type),
              sender_id(sender_id){}

    ProceededEvent(const string &channelName,
                 const MsgType type,
                 SocketProtoBuffer *socketProtoBuffer,
                 const uint32_t chunk,
                 const uint32_t num_of_chunks,
                 const uint32_t sender_id) :
            channelName(channelName),
            type(type),
            socketProtoBuffer(socketProtoBuffer),
            chunk(chunk),
            num_of_chunks(num_of_chunks),
            sender_id(sender_id) {}

    inline const string getChannelName() const {
        return channelName;
    }

    inline const MsgType getType() const {
        return type;
    }

    inline SocketProtoBuffer *getSocketProtoBuffer() {
        return socketProtoBuffer;
    }

    inline const uint32_t getSender_id() const {
        return sender_id;
    }

    inline const uint32_t getChunk() const {
        return chunk;
    }

    inline const uint32_t getNumOfChunks() const {
        return num_of_chunks;
    }
    inline void releaseBufferList(){
        BufferPool::bufferPool->releaseList(socketProtoBuffer);
    }
private:
    const string channelName;
    const MsgType type;
    SocketProtoBuffer *socketProtoBuffer = nullptr;
    const uint32_t sender_id;


private:
    const uint32_t chunk = 0;
    const uint32_t num_of_chunks = 0;


};


#endif //TCP_SHMAFKA_PROCEEDEDMSG_H
