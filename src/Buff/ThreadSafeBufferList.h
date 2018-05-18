//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_THREADSAFEBUFFERLIST_H
#define TCP_SHMAFKA_THREADSAFEBUFFERLIST_H


#include "BuffersList.h"

class ThreadSafeBufferList : public BuffersList{
public:
    explicit ThreadSafeBufferList(SocketProtoBuffer *head) : BuffersList(head) {}

    ThreadSafeBufferList() = default;

    uint32_t append(SocketProtoBuffer *buff) override {
        unique_lock<mutex> lock(rwMutex);
        return BuffersList::append(buff);
    }

    SocketProtoBuffer * get_head() override {
        unique_lock<mutex> lock(rwMutex);
        return BuffersList::get_head();
    }

    SocketProtoBuffer *remove_head() override {
        unique_lock<mutex> lock(rwMutex);
        return BuffersList::remove_head();
    }

    uint32_t get_len() override {
        unique_lock<mutex> lock(rwMutex);
        return BuffersList::get_len();
    }

    SocketProtoBuffer *get_tail() override {
        unique_lock<mutex> lock(rwMutex);
        return BuffersList::get_tail();
    }

private:
    std::mutex rwMutex{};
};


#endif //TCP_SHMAFKA_THREADSAFEBUFFERLIST_H
