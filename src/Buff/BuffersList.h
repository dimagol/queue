//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_BUFFERSLIST_H
#define TCP_SHMAFKA_BUFFERSLIST_H


#include "SocketProtoBuffer.h"
#include "../Logging/TSLogger.h"
#include "BufferPool.h"

class BuffersList {

public:
    BuffersList() :head(nullptr),
                   tail(nullptr),
                   len(0){}

    explicit BuffersList(SocketProtoBuffer *buff) :
            head(buff),
            tail(buff),
            len(1) {
        if(buff->nextBuffer != nullptr){
            LOG_ERROR("logic issue...")
        }
    }

    virtual uint32_t append(SocketProtoBuffer *buff){
        if(buff->nextBuffer != nullptr){
            LOG_ERROR("logic issue...")
        }
        if(tail == nullptr){
            tail = buff;
            head = buff;
            len = 1;
        } else {
            tail->nextBuffer = buff;
            tail = buff;
            len++;
        }

        return len;
    }

    virtual SocketProtoBuffer * get_head() {
        return head;
    }

    virtual SocketProtoBuffer * get_tail(){
        return tail;
    }


    virtual SocketProtoBuffer * remove_head(){
        SocketProtoBuffer * tmp = head;
        head = head->nextBuffer;
        len--;
        return tmp;
    }

    virtual uint32_t get_len(){
        return len;
    }

    virtual ~BuffersList() {
        BufferPool::bufferPool->releaseAllChain(head);
    }
    BuffersList * nextList = nullptr;
private:

    SocketProtoBuffer * head;
    SocketProtoBuffer * tail;
    uint32_t len;
};


#endif //TCP_SHMAFKA_BUFFERSLIST_H
