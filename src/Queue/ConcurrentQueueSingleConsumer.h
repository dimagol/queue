//
// Created by dima on 25/01/18.
//

#ifndef TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H
#define TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H

#include <cstdint>
#include <mutex>
#include <queue>
#include "IConcurrentQueue.h"
#include "QueueWaitingStrategy.h"
using namespace std;

template <typename T>
class ConcurrentQueueSingleConsumer:public IConcurrentQueue<T>{
public:
    ConcurrentQueueSingleConsumer(uint32_t maxSize,
                                  Strategy strategy):
            maxSize(maxSize),
            waitingStrategy(getStrategy(strategy)),
            consumerQueue(new queue<T>()),
            lockedQueue(new queue<T>())
    {};

    T pop() override {
        if (consumerQueue->empty()){
            mutex.lock();
            while (lockedQueue->empty()){
                mutex.unlock();
                waitingStrategy->wait();
                mutex.lock();
            }
            swap(lockedQueue,consumerQueue);
            mutex.unlock();

        }
        auto ret = consumerQueue->front();
        consumerQueue->pop();
        return ret;
    }

    T try_pop() override {
        if (consumerQueue->empty()){
            mutex.lock();
            if (lockedQueue->empty()){
                mutex.unlock();
                return T();
            } else{
                swap(lockedQueue,consumerQueue);
                mutex.unlock();
                auto ret = consumerQueue->front();
                consumerQueue->pop();
                return ret;
            }

        } else {
            auto ret = consumerQueue->front();
            consumerQueue->pop();
            return ret;
        }
    }

    void push(T &item) override {
        mutex.lock();
        while (lockedQueue->size() >= maxSize){
            mutex.unlock();
            waitingStrategy->wait();
            mutex.lock();
        }
        lockedQueue->push(item);
        mutex.unlock();
    }

private:
    uint32_t maxSize;
    std::queue<T> *lockedQueue;
    std::queue<T> *consumerQueue;
    std::mutex mutex;
    IQueueWaitingStrategy *waitingStrategy;
};
#endif //TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H
