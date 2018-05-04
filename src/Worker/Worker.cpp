//
// Created by dima on 07/12/17.
//

#include "Worker.h"


void Worker::run() {
    if (channelDb == nullptr || producerServer == nullptr || consumerServer == nullptr || msgBuilder == nullptr ){
        LOG_ERROR("not set");
        return;
    }
    while (shouldRun){
        auto prodMsg = producerServer->tryRecieve();

        if(prodMsg != nullptr){
//            prodMsg->releaseBuffer();
         auto event = processor.getEventByBuff(*prodMsg);
            if(event.getType() != MsgType::UNDEFINED){
                handlePostMsg(event);
            } else {
                LOG_ERROR("got undefined msg")
            }


        }

        auto consMsg = consumerServer->tryRecieve();
        if(consMsg != nullptr){
//            consMsg ->releaseBuffer();
            auto event = processor.getEventByBuff(*consMsg);
            if(event.getType() != MsgType::UNDEFINED){
                handleListenMsg(event);
            } else {
                LOG_ERROR("got undefined msg")
            }
        }

        if(consMsg == nullptr && prodMsg == nullptr){
            waitingStrategy->wait();
        }
    }
    LOG_INFO("worker done")
}

void Worker::handlePostMsg(ProceededEvent &event) const {
    switch (event.getType()){
        case POST_REGISTER:
            handlePostRegister(event);
            break;
        case POST_POST:
            handlePost(event);
            break;
        case POST_DEREGISTER:
            handlePostDeregister(event);
            break;
        case POST_DEREGISTER_ALL:
            handlePostDeregisterAll(event);
            break;
        case DISCONNECT_FROM_SERVER:
        case POST_DISCONNECT:
            handlePostDisconnect(event);
            break;
        case POST_LIST_CHANELES_REQ:
            handlePostListChannels(event);
            break;
        default:
            LOG_ERROR("got unsupported msg of type ",event.getType());
            if(event.getSocketProtoBuffer() != nullptr){
                BufferPool::bufferPool->release(event.getSocketProtoBuffer());
            }
    }
}

void Worker::handleListenMsg(ProceededEvent &event) const {
    switch (event.getType()){
        case LISTEN_REGISTER:
            handleListenRegister(event);
            break;
        case LISTEN_DEREGISTER:
            handleListenDeregister(event);
            break;
        case LISTEN_DEREGISTER_ALL:
            handleListenDeregisterAll(event);
            break;
        case DISCONNECT_FROM_SERVER:
        case LISTEN_DISCONNECT:
            handleListenDisconnect(event);
            break;
        case LISTEN_LIST_CHANELES_REQ:
            handleListenListChannels(event);
            break;
        default:
            LOG_ERROR("got unsupported msg of type ",event.getType());
            if(event.getSocketProtoBuffer() != nullptr){
                BufferPool::bufferPool->release(event.getSocketProtoBuffer());
            }
    }
}

// post handlers
void Worker::handlePostRegister(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr){
        channel = channelDb->createChannel(event.getChannelName());
    }
    channel->feed(event);
}

void Worker::handlePost(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr) {
        LOG_ERROR("channel not exist ", event.getChannelName());
        if(event.getSocketProtoBuffer() != nullptr){
            BufferPool::bufferPool->release(event.getSocketProtoBuffer());
        }
    } else {
        channel->feed(event);
        if (channel->haveNewData()) {
            auto data = channel->getBuffDone();
            channel->setNoData();
            auto ptr = make_shared<TcpServerOutcomeMessage>(data->head,
                                                            channel->getRegisteredUsers());
            consumerServer->send(ptr);
        }
    }
}

void Worker::handlePostDeregister(ProceededEvent &event) const {
    auto channel = this->channelDb->getChannel(event.getChannelName());
    if (channel != nullptr) {
        channel->feed(event);
    }
}

void Worker::handlePostDeregisterAll(ProceededEvent &event) const {
    auto channelVec = this->channelDb->getAllPostChannelsForId(event.getSender_id());
    for (auto channel : *channelVec) {
        channel->feed(event);
    }
}


void Worker::handlePostDisconnect(ProceededEvent &event) const {
    if (event.getType() == DISCONNECT_FROM_SERVER){
        ProceededEvent event1(POST_DISCONNECT,event.getSender_id());
        handlePostDeregisterAll(event1);
    } else{
        handlePostDeregisterAll(event);
        producerServer->disconnectClient(event.getSender_id());
    }

}

void Worker::handlePostListChannels(ProceededEvent &event) const {
    string channelsStr = this->channelDb->getChannelListStr();
    auto buff = msgBuilder->buildListenListChannelsResMsg(channelsStr);
    if (buff != nullptr) {
        auto ptr = make_shared<TcpServerOutcomeMessage>(buff, event.getSender_id());
        this->producerServer->send(ptr);
    } else{
        LOG_ERROR("got null buff");
    }
}

// listen handlers
void Worker::handleListenRegister(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr){
        channel = channelDb->createChannel(event.getChannelName());
    }
    channel->feed(event);
}

void Worker::handleListenDeregister(ProceededEvent &event) const {
    auto channel = this->channelDb->getChannel(event.getChannelName());
    if (channel != nullptr) {
        channel->feed(event);
    }
}

void Worker::handleListenDeregisterAll(ProceededEvent &event) const {
    auto channelVec = this->channelDb->getAllListenChannelsForId(event.getSender_id());
    for (const auto &channel : *channelVec) {
        channel->feed(event);
    }
}

void Worker::handleListenDisconnect(ProceededEvent &event) const {
    if (event.getType() == DISCONNECT_FROM_SERVER){
        ProceededEvent event1(LISTEN_DISCONNECT,event.getSender_id());
        handleListenDeregisterAll(event1);
    } else {
        handleListenDeregisterAll(event);
        consumerServer->disconnectClient(event.getSender_id());
    }

}

void Worker::handleListenListChannels(ProceededEvent &event) const {
    string str = this->channelDb->getChannelListStr();
    auto buff = msgBuilder->buildListenListChannelsResMsg(str);
    if (buff != nullptr) {
        auto ptr = make_shared<TcpServerOutcomeMessage>(buff, event.getSender_id());
        this->consumerServer->send(ptr);
    } else{
        LOG_ERROR("got null buff");
    }
}

// setters
void Worker::setChannelDb(ChannelDb * channelDb) {
    Worker::channelDb = channelDb;
}

void Worker::setProducerServer(TcpServer* producerServer) {
    Worker::producerServer = producerServer;
}

void Worker::setConsumerServer(TcpServer* consumerServer) {
    Worker::consumerServer = consumerServer;
}

void Worker::setShouldRun(volatile bool shouldRun) {
    Worker::shouldRun = shouldRun;
}

void Worker::setBuilder(MsgBuilder *builder) {
    Worker::msgBuilder = builder;
}

void Worker::setWaitingStrategy(WaitingStrategy *waitingStrategy) {
    Worker::waitingStrategy = waitingStrategy;
}
