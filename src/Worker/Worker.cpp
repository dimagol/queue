//
// Created by dima on 07/12/17.
//

#include "Worker.h"

void Worker::setChannelDb(const shared_ptr<ChannelDb> &channelDb) {
    Worker::channelDb = channelDb;
}

void Worker::setProducerServer(const shared_ptr<TcpServer> &producerServer) {
    Worker::producerServer = producerServer;
}

void Worker::setConsumerServer(const shared_ptr<TcpServer> &consumerServer) {
    Worker::consumerServer = consumerServer;
}

void Worker::setShouldRun(volatile bool shouldRun) {
    Worker::shouldRun = shouldRun;
}
