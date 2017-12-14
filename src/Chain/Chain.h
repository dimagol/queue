//
// Created by dima on 14/12/17.
//

#ifndef TCP_SHMAFKA_CHAIN_H
#define TCP_SHMAFKA_CHAIN_H


#include "../Threads/WorkerThread.h"
#include "../Threads/ServerThread.h"

class Chain {
public:
    Chain(uint16_t consumerPort, uint16_t producerPort) :
            consumerPort(consumerPort),
            producerPort(producerPort),
            consumerTcpServer(consumerPort),
            producerTcpServer(producerPort)
    {
        worker.setShouldRun(true);
        worker.setChannelDb(&channelDb);
        worker.setConsumerServer(&consumerTcpServer);
        worker.setProducerServer(&producerTcpServer);

        workerThread.init(&worker);
        consumerServerThread.init(&consumerTcpServer);
        producerServerThread.init(&producerTcpServer);
    }

    void runThreads(){
        consumerServerThread.run();
        producerServerThread.run();
        workerThread.run();
    }

    void stop(){
    }

private:
    WorkerThread workerThread;
    Worker worker;
    uint16_t consumerPort;
    uint16_t producerPort;
    ServerThread consumerServerThread;
    ServerThread producerServerThread;
    TcpServer consumerTcpServer;
    TcpServer producerTcpServer;
    ChannelDb channelDb;
};


#endif //TCP_SHMAFKA_CHAIN_H
