//
// Created by dima on 25/01/18.
//

#ifndef TCP_SHMAFKA_ICONCURRENTQUEUE_H
#define TCP_SHMAFKA_ICONCURRENTQUEUE_H

template <typename T>
class IConcurrentQueue
{
public:
    virtual T pop() = 0;

    virtual T try_pop() = 0;

    virtual void push(T& item) = 0;

};
#endif //TCP_SHMAFKA_ICONCURRENTQUEUE_H
