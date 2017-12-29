//
// Created by dima on 13/10/17.
//

#include "BufferPool.h"
#include "../Logging/TSLogger.h"
#include "../Msg/MsgConsts.h"

BufferPool* BufferPool::bufferPool = BufferPool::create(1024*64, 1024);


BufferPool::BufferPool(uint32_t total, uint32_t bufferLen) : total(total), bufferLen(bufferLen) {
    for (int i = 0; i < total; i++){
        bufferVector.push_back(new SocketProtoBuffer(bufferLen));
    }
}

BufferPool* BufferPool::create(uint32_t total, uint32_t bufferLen){
    return new BufferPool(total, bufferLen);
}





SocketProtoBuffer *BufferPool::getChunkedWithIntAndStr(uint32_t type, string &str) {
    return getChunkedWithIntAndData(type, (uint8_t *) str.c_str(), (uint32_t) str.size() + 1);

}

SocketProtoBuffer *BufferPool::getChunkedWithIntAndChannelStr(uint32_t type, const string &channel,const  string &str) {
    if(channel.size() > 16){
        LOG_ERROR("bad channel len ",channel);
        return nullptr;
    }
    return getChunkedWithIntAndChannelData(type,
                                           (uint8_t *)channel.c_str(),
                                           (uint32_t) channel.size() + 1,
                                           (uint8_t *) str.c_str(),
                                           (uint32_t) str.size() + 1);

}


SocketProtoBuffer *BufferPool::getChunkedWithIntAndChannelData(uint32_t type,
                                                               uint8_t * channel,
                                                               uint32_t channelLen,
                                                               uint8_t *data,
                                                               uint32_t dataLen) {
    uint32_t chunkNumber = 1;
    uint32_t dataLenPerBuffer = bufferLen - sizeof(type) - sizeof(uint32_t) - sizeof(chunkNumber) - CONSTS_POST_CHANEL_LEN;
    uint32_t  numOfBuffers = dataLen / dataLenPerBuffer;
    if (dataLen % dataLenPerBuffer != 0){
        numOfBuffers++;
    }
    auto ret = getLinked(numOfBuffers);
    if(ret == nullptr){
        return nullptr;
    }
    auto tmp = ret;
    uint32_t dataOffset = 0;
    for(int i = 0; i < numOfBuffers ; i++){
        tmp->append_int(type,CONSTS_MSG_TYPE_OFFSET);
        tmp->append_data(channel,channelLen,CONSTS_POST_CHANEL_OFFSET);
        tmp->append_int(chunkNumber,CONSTS_POST_MSG_CHUNK_OFFSET);
        chunkNumber++;
        tmp->append_int(numOfBuffers,CONSTS_POST_MSG_NUM_OF_CHUNKS_OFFSET);
        if(i == (numOfBuffers -1) && dataLen % dataLenPerBuffer != 0){
            dataLenPerBuffer = dataLen % dataLenPerBuffer;
        }
        tmp->append_data(data + dataOffset, dataLenPerBuffer,CONSTS_POST_MSG_DATA_OFFSTE);
        dataOffset+=dataLenPerBuffer;
        tmp = tmp->nextBuffer;
    }
    return ret;
}
SocketProtoBuffer *BufferPool::getChunkedWithIntAndData(uint32_t type, const uint8_t *data, uint32_t len) {
    uint32_t numOfChunks = 0;
    uint32_t chunkNumber = 0;
    uint32_t dataLenPerBuffer = bufferLen - sizeof(type) - sizeof(numOfChunks) - sizeof(chunkNumber);
    uint32_t  numOfBuffers = len / dataLenPerBuffer;
    if (len % dataLenPerBuffer != 0){
        numOfBuffers++;
    }
    auto ret = getLinked(numOfBuffers);
    if(ret == nullptr){
        return nullptr;
    }
    auto tmp = ret;
    uint32_t dataOffset = 0;
    for(int i = 0; i < numOfBuffers ; i++){
        tmp->append_int(type,0);
        tmp->append_int(numOfChunks,4);
        tmp->append_int(chunkNumber,8);
        if(i == (numOfBuffers -1) && len % dataLenPerBuffer != 0){
            dataLenPerBuffer = len % dataLenPerBuffer;
        }
        tmp->append_data((uint8_t *)data + dataOffset, dataLenPerBuffer,12);
        dataOffset+=dataLenPerBuffer;
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
//    cout << "get vec size " << bufferVector.size() << " instanse " << ret << endl;
    bufferVector.pop_back();
    for(int i = 0; i < num -1; i++){
        tail->nextBuffer = bufferVector.back();
        bufferVector.pop_back();
        tail = tail->nextBuffer;
    }
    lock.unlock();
    return ret;
}

SocketProtoBuffer *BufferPool::get() {
    lock.lock();

    if (bufferVector.empty()){
        lock.unlock();
        LOG_ERROR("no buffers");
        return nullptr;
    }

    SocketProtoBuffer * ret = bufferVector.back();
    bufferVector.pop_back();
    lock.unlock();
    return ret;
}

void BufferPool::release(SocketProtoBuffer * buffer) {
    lock.lock();
    auto next = buffer;
    while (next != nullptr){
        auto tmp = next;
        next = next->nextBuffer;
        tmp->reset();
//        cout << "rel vec size " << bufferVector.size() << "instanse " << tmp <<endl;
        bufferVector.push_back(tmp);
    }
    lock.unlock();
}

void BufferPool::releaseOne(SocketProtoBuffer * buffer) {
    lock.lock();
    buffer->reset();
    bufferVector.push_back(buffer);
    lock.unlock();
}

SocketProtoBuffer *BufferPool::getWithIntAndData(uint32_t type, const uint8_t *data, uint32_t len) {
    auto buff = get();
    if(buff == nullptr){
        LOG_ERROR("cant get buff")
        return nullptr;
    }
    buff->append_int(type,0);
    buff->append_data(data,len,4);
    return buff;
}

SocketProtoBuffer *BufferPool::getWithIntAndStr(uint32_t type, const string &str) {
    return getWithIntAndData(type,(uint8_t *)str.c_str(), (uint32_t)str.size()+1);
}

SocketProtoBuffer *BufferPool::getWithInt(uint32_t type) {
    auto buff = get();
    if(buff == nullptr){
        LOG_ERROR("cant get buff")
        return nullptr;
    }
    buff->append_int(type,0);
    return buff;
}
