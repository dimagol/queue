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
        lock.unlock();
        return nullptr;
    }
    SocketProtoBuffer * ret = bufferVector.back();
    bufferVector.pop_back();
    lock.unlock();
    return ret;
}

void BufferPool::release(SocketProtoBuffer * buffer) {
    buffer->reset();
    lock.lock();
    bufferVector.push_back(buffer);
    lock.unlock();
}

void BufferPool::releaseList(SocketProtoBuffer *buffer) {
    lock.lock();
    auto next = buffer;
    while (next != nullptr){
        auto tmp = next;
        next = next->nextBuffer;
        tmp->reset();
        bufferVector.push_back(tmp);
    }
    lock.unlock();
}

BufferPool* BufferPool::bufferPool = BufferPool::create(1024, 65535);

SocketProtoBuffer *BufferPool::getChunkedBuffer(uint32_t type, const char *str, uint32_t len) {
    uint32_t numOfChunks = 0;
    uint32_t chunkNumber = 0;
    uint32_t dataLenPerBuffer = bufferLen - sizeof(type) - sizeof(numOfChunks) - sizeof(chunkNumber);
    uint32_t  numOfBuffers = len / dataLenPerBuffer;
    if (len % dataLenPerBuffer != 0){
        numOfBuffers++;
    }
    auto ret = getLinked(numOfBuffers);
    auto tmp = ret;
    for(int i = 0; i < numOfBuffers ; i++){
        tmp->append_int(type,0);
        tmp->append_int(numOfChunks,4);
        tmp->append_int(chunkNumber,8);
        if(i == (numOfBuffers -1) && len % dataLenPerBuffer != 0){
            dataLenPerBuffer = len % dataLenPerBuffer;
        }
        tmp->append_data((uint8_t *)str, dataLenPerBuffer,12);
        tmp = tmp->nextBuffer;
    }
    return ret;
}

SocketProtoBuffer *BufferPool::getLinked(uint32_t num) {
    lock.lock();
    if (bufferVector.size() < num){
        lock.unlock();
        return nullptr;
    }
    SocketProtoBuffer * ret = bufferVector.back();
    SocketProtoBuffer * tail = ret;
    bufferVector.pop_back();
    for(int i = 0; i < num -1; i++){
        tail->nextBuffer = bufferVector.back();
        bufferVector.pop_back();
        tail = tail->nextBuffer;
    }
    lock.unlock();
    return ret;
}
