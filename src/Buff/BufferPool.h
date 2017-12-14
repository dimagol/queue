//
// Created by dima on 13/10/17.
//

#include <cstdint>
#include <vector>
#include <netinet/in.h>
#include <mutex>
#include <cstring>
#include <ostream>
#include <iostream>
#include "SocketProtoBuffer.h"

#ifndef TCP_SHMAFKA_BUFFERPOLL_H
#define TCP_SHMAFKA_BUFFERPOLL_H
using namespace std;


class BufferPool{
public:
    BufferPool(uint32_t total, uint32_t bufferLen);

    SocketProtoBuffer* get();
    SocketProtoBuffer* getWithIntAndData(uint32_t type, const uint8_t *data, uint32_t len);
    SocketProtoBuffer* getWithIntAndStr(uint32_t type, string &str);
    SocketProtoBuffer* getWithInt(uint32_t type);
    void release(SocketProtoBuffer *);
    void releaseList(SocketProtoBuffer *buffer);
    SocketProtoBuffer *getChunkedWithIntAndData(uint32_t type, const uint8_t *str, uint32_t len);
    SocketProtoBuffer *getChunkedWithIntAndStr(uint32_t type, string &str);
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


};

#endif //TCP_SHMAFKA_BUFFERPOLL_H