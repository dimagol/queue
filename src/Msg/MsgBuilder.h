//
// Created by dima on 14/12/17.
//

#ifndef TCP_SHMAFKA_MSGBUILDER_H
#define TCP_SHMAFKA_MSGBUILDER_H


#include "../Buff/BufferPool.h"
#include "MsgType.h"
#include "../Logging/TSLogger.h"

class MsgBuilder {
public:
    explicit MsgBuilder(BufferPool *bufferPool);

    SocketProtoBuffer * buildPostRegistrationMsg(const string &channel){
        if(channel.size() > 16){
            LOG_ERROR("got to big channel name ", channel)
            return nullptr;
        }
        return bufferPool->getWithIntAndStr(POST_REGISTER,channel);
    }

    SocketProtoBuffer * buildPostPostChannelsResMsg(const string &channel, const string &dataStr) {
        return bufferPool->getChunkedWithIntAndChannelStr(POST_POST, channel, dataStr);
    }

    SocketProtoBuffer * buildPostPostChannelsResMsg(const uint8_t * data, uint32_t len) {
        return bufferPool->getChunkedWithIntAndData(POST_POST, data, len);
    }

    SocketProtoBuffer * buildPostDeregistrationMsg(string &channel){
        if(channel.size() > 16){
            LOG_ERROR("got to big channel name ", channel)
            return nullptr;
        }
        return bufferPool->getWithIntAndStr(POST_DEREGISTER,channel);
    }

    SocketProtoBuffer * buildPostDeregistrationAllMsg(){
        return bufferPool->getWithInt(POST_DEREGISTER_ALL);
    }


    SocketProtoBuffer * buildPostListChannelsReqMsg() {
        return bufferPool->getWithInt(POST_LIST_CHANELES_REQ);
    }


    SocketProtoBuffer * buildPostListChannelsResMsg(string &channels) {
        return bufferPool->getChunkedWithIntAndStr(POST_LIST_CHANELES_RES, channels);
    }

    SocketProtoBuffer * buildListenRegistrationMsg(const string &channel){
        if(channel.size() > 16){
            LOG_ERROR("got to big channel name ", channel)
            return nullptr;
        }
        return bufferPool->getWithIntAndStr(LISTEN_REGISTER,channel);
    }

    SocketProtoBuffer * buildListenPostResMsg(string &dataStr) {
        return bufferPool->getChunkedWithIntAndStr(LISTEN_POST, dataStr);
    }

    SocketProtoBuffer * buildListenPostResMsg(const uint8_t * data, uint32_t len) {
        return bufferPool->getChunkedWithIntAndData(LISTEN_POST, data, len);
    }

    SocketProtoBuffer * buildListenDeregistrationMsg(string &channel){
        if(channel.size() > 16){
            LOG_ERROR("got to big channel name ", channel)
            return nullptr;
        }
        return bufferPool->getWithIntAndStr(LISTEN_DEREGISTER,channel);
    }

    SocketProtoBuffer * buildListenDeregistrationAllMsg(){
        return bufferPool->getWithInt(LISTEN_DEREGISTER_ALL);
    }

    SocketProtoBuffer * buildListenListChannelsReqMsg() {
        return bufferPool->getWithInt(LISTEN_LIST_CHANELES_REQ);
    }

    SocketProtoBuffer * buildListenListChannelsResMsg(string &channels) {
        return bufferPool->getChunkedWithIntAndStr(LISTEN_LIST_CHANELES_RES, channels);
    }

    SocketProtoBuffer * buildDisconnectMsg(){
        return bufferPool->getWithInt(DISCONNECT_FROM_SERVER);
    }

    SocketProtoBuffer * buildPostDisconnectMsg(){
        return bufferPool->getWithInt(POST_DISCONNECT);
    }
    SocketProtoBuffer * buildListenDisconnectMsg(){
        return bufferPool->getWithInt(LISTEN_DISCONNECT);
    }


private:
    BufferPool* bufferPool;
};


#endif //TCP_SHMAFKA_MSGBUILDER_H
