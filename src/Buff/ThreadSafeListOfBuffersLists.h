//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_THREADSAFELISTOFBUFFERSLISTS_H
#define TCP_SHMAFKA_THREADSAFELISTOFBUFFERSLISTS_H


#include "ListOfBufferLists.h"

class ThreadSafeListOfBuffersLists : public ListOfBufferLists {
public:
    ThreadSafeListOfBuffersLists() : ListOfBufferLists() {

    }

    explicit ThreadSafeListOfBuffersLists(BuffersList *buffersList) : ListOfBufferLists(buffersList) {

    }

    uint32_t append(BuffersList *buffersList) override {
        unique_lock<mutex> lock(rwMutex);
        return ListOfBufferLists::append(buffersList);
    }

    BuffersList *get_head() override {
        unique_lock<mutex> lock(rwMutex);
        return ListOfBufferLists::get_head();
    }

    BuffersList *get_tail() override {
        unique_lock<mutex> lock(rwMutex);
        return ListOfBufferLists::get_tail();
    }

    BuffersList *remove_head() override {
        unique_lock<mutex> lock(rwMutex);
        return ListOfBufferLists::remove_head();
    }

    uint32_t get_len() override {
        unique_lock<mutex> lock(rwMutex);
        return ListOfBufferLists::get_len();
    }

    ~ThreadSafeListOfBuffersLists() {

    }

private:
    std::mutex rwMutex{};

};


#endif //TCP_SHMAFKA_THREADSAFELISTOFBUFFERSLISTS_H
