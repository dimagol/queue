//
// Created by dima on 13/10/17.
//



#ifndef TCP_SHMAFKA_BUFFERPOLL_H
#define TCP_SHMAFKA_BUFFERPOLL_H

#include <cstdint>
#include <vector>
#include <netinet/in.h>
#include <mutex>
#include <cstring>
#include <ostream>
#include <iostream>
#include "SocketProtoBuffer.h"


using namespace std;

class BufferPool{
public:
    BufferPool(uint32_t total, uint32_t bufferLen);

    SocketProtoBuffer* get();
//    SocketProtoBuffer* getWithIntAndData(uint32_t type, const uint8_t *data, uint32_t len);
    SocketProtoBuffer* getWithIntAndStr(uint32_t type, const string &str);
//    SocketProtoBuffer* getWithInt(uint32_t type);
    void releaseAllChain(SocketProtoBuffer *);
    void releaseOne(SocketProtoBuffer *buffer);
//    SocketProtoBuffer *getChunkedWithIntAndData(uint32_t type, const uint8_t *str, uint32_t len);
//    SocketProtoBuffer *getChunkedWithIntAndStr(uint32_t type, string &str);

//    SocketProtoBuffer *getChunkedWithIntAndChannelStr(uint32_t type, const string &channel, const string &str);
//    SocketProtoBuffer *getChunkedWithIntAndChannelData(uint32_t type, uint8_t *channel, uint32_t channelLen, uint8_t *data, uint32_t dataLen);
    virtual ~BufferPool() {

    }
    static BufferPool* create(uint32_t total, uint32_t bufferLen);
    static BufferPool* bufferPool;
private:
    SocketProtoBuffer* getLinked(uint32_t num);
    mutex lock;
    vector<SocketProtoBuffer *> bufferVector;
    uint32_t total;
    uint32_t bufferLen;
public:
    uint32_t getBufferLen() const;

public:
    int getSize();
};

#endif //TCP_SHMAFKA_BUFFERPOLL_H