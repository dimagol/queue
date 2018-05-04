//
// Created by dima on 25/01/18.
//

#ifndef TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H
#define TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H

#include <cstdint>
#include <mutex>
#include <queue>
#include "IConcurrentQueue.h"
#include "../WaitingStrategy.h"
using namespace std;

template <typename T>
class ConcurrentQueueSingleConsumer:public IConcurrentQueue<T>{
public:
    ConcurrentQueueSingleConsumer(uint32_t maxSize,
                                  StrategyType strategy);;

    T pop() override;

    T try_pop() override;

    void push(T &item) override;

    void push(T &&item) override;

    virtual ~ConcurrentQueueSingleConsumer();

private:
    uint32_t maxSize;
    std::queue<T> *lockedQueue;
    std::queue<T> *consumerQueue;
    std::mutex mutex;
    WaitingStrategy *waitingStrategy;
};

template<typename T>
T ConcurrentQueueSingleConsumer<T>::pop() {
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

template<typename T>
ConcurrentQueueSingleConsumer<T>::~ConcurrentQueueSingleConsumer() {
    delete lockedQueue;
    delete consumerQueue;
    delete waitingStrategy;
}
template<typename T>
T ConcurrentQueueSingleConsumer<T>::try_pop() {
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
template<typename T>
void ConcurrentQueueSingleConsumer<T>::push(T &item) {
    mutex.lock();
    while (lockedQueue->size() >= maxSize){
        mutex.unlock();
        waitingStrategy->wait();
        mutex.lock();
    }
    lockedQueue->push(item);
    mutex.unlock();
}
template<typename T>
void ConcurrentQueueSingleConsumer<T>::push(T &&item) {
    mutex.lock();
    while (lockedQueue->size() >= maxSize){
        mutex.unlock();
        waitingStrategy->wait();
        mutex.lock();
    }
    lockedQueue->push(std::move(item));
    mutex.unlock();

}
template<typename T>
ConcurrentQueueSingleConsumer<T>::ConcurrentQueueSingleConsumer(uint32_t maxSize, StrategyType strategy) :
        maxSize(maxSize),
        waitingStrategy(getStrategy(strategy)),
        consumerQueue(new queue<T>()),
        lockedQueue(new queue<T>())
{}


#endif //TCP_SHMAFKA_CONCURRENTQUEUESINGLECONSUMER_H
