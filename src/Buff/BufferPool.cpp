//
// Created by dima on 13/10/17.
//

#include "BufferPool.h"
BufferPool* BufferPool::create(uint32_t total, uint32_t bufferLen){
    return new BufferPool(total, bufferLen);
}


BufferPool::BufferPool(uint32_t total, uint32_t bufferLen) : total(total), bufferLen(bufferLen) {
    for (int i = 0; i < total; i++){
        bufferVector.push_back(new SocketProtoBuffer(bufferLen));
    }
}

SocketProtoBuffer *BufferPool::get() {
    lock.lock();
    if (bufferVector.empty()){
        return nullptr;
    }
    SocketProtoBuffer * ret = bufferVector.back();
    bufferVector.pop_back();
    lock.unlock();
    return ret;
}

void BufferPool::release(SocketProtoBuffer * buffer) {
    lock.lock();
    bufferVector.push_back(buffer);
    lock.unlock();
}

BufferPool* BufferPool::bufferPool = BufferPool::create(1024, 65535);
