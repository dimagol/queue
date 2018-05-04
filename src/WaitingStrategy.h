//
// Created by dima on 27/01/18.
//

#ifndef TCP_SHMAFKA_WAITINGSTRATEGY_H
#define TCP_SHMAFKA_WAITINGSTRATEGY_H

#include <unistd.h>
#include <thread>

using namespace std;
enum StrategyType{SLEEP,YIELD,POLLING};
class WaitingStrategy{
public:

    virtual void wait() = 0;
};

class SleepQueueWaitingStrategy: public WaitingStrategy{
public:
    void wait() override {
        usleep(50);
    }
};

class YeldQueueWaitingStrategy: public WaitingStrategy{
public:
    void wait() override {
        this_thread::yield();
    }
};

class PollingQueueWaitingStrategy: public WaitingStrategy{
public:
    void wait() override {
    }
};

WaitingStrategy * getStrategy(StrategyType strategy);




#endif //TCP_SHMAFKA_WAITINGSTRATEGY_H
