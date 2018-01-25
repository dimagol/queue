//
// Created by dima on 25/01/18.
//

#ifndef TCP_SHMAFKA_QUEUEWAITINGSTRATEGY_H
#define TCP_SHMAFKA_QUEUEWAITINGSTRATEGY_H
#include <unistd.h>
#include <thread>

using namespace std;
enum Strategy{SLEEP,YIELD};
class IQueueWaitingStrategy{
public:

    virtual void wait() = 0;
};

class SleepQueueWaitingStrategy: public IQueueWaitingStrategy{
public:
    void wait() override {
        usleep(1);
    }
};

class YeldQueueWaitingStrategy: public IQueueWaitingStrategy{
public:
    void wait() override {
        this_thread::yield();
    }
};

IQueueWaitingStrategy * getStrategy(Strategy strategy){
    switch (strategy){
        case Strategy::SLEEP:
            return new SleepQueueWaitingStrategy();
        case Strategy::YIELD:
            return new YeldQueueWaitingStrategy();
    }
}

#endif //TCP_SHMAFKA_QUEUEWAITINGSTRATEGY_H
