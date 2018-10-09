//
// Created by dima on 14/12/17.
//

#ifndef TCP_SHMAFKA_WORKERTHREAD_H
#define TCP_SHMAFKA_WORKERTHREAD_H

#include <thread>
#include "../Logging/TSLogger.h"
#include "../Worker/Worker.h"

using namespace std;

class WorkerThread {
public:
    void init(Worker *worker_ptr){
        LOG_INFO("worker init");
        worker = worker_ptr;

    }
    void run(){
        LOG_INFO("worker run");
        workerThread = new thread(&Worker::run, worker);
    }

    void join(){
        workerThread->join();
    }

    virtual ~WorkerThread() {
        LOG_INFO("worker end");
        worker->setShouldRun(false);
        workerThread->join();
        delete workerThread;
    }

private:
    std::thread *workerThread = nullptr;
    Worker *worker = nullptr;
};


#endif //TCP_SHMAFKA_WORKERTHREAD_H
