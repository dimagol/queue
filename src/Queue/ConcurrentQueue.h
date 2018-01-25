//
// Created by dima on 04/10/17.
//

#ifndef SHMAFKA_CONCURENTQUEUE_H
#define SHMAFKA_CONCURENTQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "IConcurrentQueue.h"

template <typename T>
class ConcurrentQueue : public IConcurrentQueue<T>
{
public:

    ConcurrentQueue() {}

    T pop() override
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    T try_pop() override
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        if (queue_.empty())
        {
            return nullptr;
        }
        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    void push( T& item) override
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

    virtual ~ConcurrentQueue() {

    }

    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //SHMAFKA_CONCURENTQUEUE_H
