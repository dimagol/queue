//
// Created by dima on 03/02/18.
//

#ifndef TCP_SHMAFKA_ASYNCLIMITEDQUEUE_H
#define TCP_SHMAFKA_ASYNCLIMITEDQUEUE_H

#include <cstdint>
#include <queue>
using namespace std;
template <typename T>
class AsyncLimitedQueue {
public:
    explicit AsyncLimitedQueue(uint32_t maxSize) : maxSize(maxSize) {}
    AsyncLimitedQueue():maxSize(0){};
    T try_pop() {
        if(queue_.size() != 0){
            auto item = queue_.front();
            queue_.pop();
            return item;
        }

        return T();
    }

    bool try_push(T &item) {
        if(queue_.size() > maxSize){
            return false;
        }
        queue_.push(item);
    };

    bool try_push(T &&item) {
        if(queue_.size() > maxSize){
            return false;
        }
        queue_.push(std::move(item));
    };

private:
    queue<T> queue_;
    uint32_t maxSize;
};


#endif //TCP_SHMAFKA_ASYNCLIMITEDQUEUE_H
