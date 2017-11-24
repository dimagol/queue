//
// Created by dima on 13/10/17.
//

#include <cstdint>
#include <vector>

#ifndef TCP_SHMAFKA_BUFFERPOLL_H
#define TCP_SHMAFKA_BUFFERPOLL_H

#endif //TCP_SHMAFKA_BUFFERPOLL_H

using namespace std;

class Buffer{
public:
    Buffer(uint32_t len) : len(len) , offset(0){
        buff = new uint8_t[len];
    }

    virtual ~Buffer() {
        delete(buff);
    }

    uint8_t * buff;
    uint32_t len;
private:
    uint32_t offset;
};
class BufferPool{
public:
    BufferPool(uint32_t total, uint32_t bufferLen) : total(total), bufferLen(bufferLen) {
        for (int i = 0; i < total; i++){
            bufferVector.push_back(new Buffer(bufferLen));
        }
    }
    Buffer* get(){
        if (bufferVector.empty()){
            return nullptr;
        }
        Buffer * ret = bufferVector.back();
        bufferVector.pop_back();
        return ret;
    }

    virtual ~BufferPool() {

    }

    static BufferPool* create(uint32_t total, uint32_t bufferLen);
    static BufferPool* bufferPool;
private:
    vector<Buffer *> bufferVector;
    uint32_t total;
    uint32_t bufferLen;
public:
};

