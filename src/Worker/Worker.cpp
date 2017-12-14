//
// Created by dima on 07/12/17.
//

#include "Worker.h"


void Worker::run() {
    if (channelDb == nullptr || producerServer == nullptr || consumerServer == nullptr ){
        LOG_ERROR("not set");
        return;
    }
    while (shouldRun){
        auto prodMsg = producerServer->recieve();
        if(prodMsg != nullptr){
            auto event = processor.processBuff(*prodMsg);
            if(event.getType() < MsgType::LISTEN_MSG_START){
                handlePostMsg(event);
            } else{
                handleListenMsg(event);
            }
        } else {
            LOG_ERROR("got null event")
        }
    }
    LOG_INFO("worker done")
}

void Worker::handlePostMsg(ProceededEvent &event) const {
    switch (event.getType()){
        case POST_REGISTER:
            handlePostRegister(event);
            break;
        case POST:
            handlePost(event);
            break;
        case POST_DEREGISTER:
            handlePostDeregister(event);
            break;
        case POST_DEREGISTER_ALL:
            handlePostDeregisterAll(event);
            break;
        case POST_DISCONNECT:
            handlePostDisconnect(event);
            break;
        case POST_LIST_CHANELES_REQ:
            handlePostListChannels(event);
            break;
        default:
            LOG_ERROR("");
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
        case LISTEN_DISCONNECT:
            handleListenDisconnect(event);
            break;
        case LISTEN_LIST_CHANELES_REQ:
            handleListenListChannels(event);
            break;
        default:
            LOG_ERROR("");
    }
}

// post handlers
void Worker::handlePostRegister(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr){
        channel = channelDb->createChannel(event.getChannelName());
    }
    channel->feed(event);
    event.releaseBufferList();
}

void Worker::handlePost(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr) {
        LOG_ERROR("channel not exist ", event.getChannelName());
    } else {
        channel->feed(event);
        if (channel->haveNewData()) {
            auto data = channel->getBuffDone();
            channel->setNoData();
            consumerServer->send(
                    make_shared<TcpServerOutcomeMessage>(data->head,
                                                         channel->getRegisteredUsers()));
        }
    }
}

void Worker::handlePostDeregister(ProceededEvent &event) const {
    auto channel = this->channelDb->getChannel(event.getChannelName());
    if (channel != nullptr) {
        channel->feed(event);
    }
    event.releaseBufferList();
}

void Worker::handlePostDeregisterAll(ProceededEvent &event) const {
    auto channelVec = this->channelDb->getAllPostChannelsForId(event.getSender_id());
    for (auto channel : *channelVec) {
        channel->feed(event);
    }
    event.releaseBufferList();
}

void Worker::handlePostDisconnect(ProceededEvent &event) const {
    handlePostDeregisterAll(event);
    producerServer->disconnectClient(event.getSender_id());
}

void Worker::handlePostListChannels(ProceededEvent &event) const {
    string str = this->channelDb->getChannelListStr();
    auto buff = BufferPool::bufferPool->getChunkedBuffer(POST_LIST_CHANELES_RES, str.c_str(),
                                                         str.size());
    if (buff != nullptr) {
        this->producerServer->send(make_shared<TcpServerOutcomeMessage>(buff, event.getSender_id()));
    } else{
        LOG_ERROR("got null buff");
    }
    event.releaseBufferList();
}

// listen handlers
void Worker::handleListenRegister(ProceededEvent &event) const {
    auto channel = channelDb->getChannel(event.getChannelName());
    if (channel == nullptr){
        channel = channelDb->createChannel(event.getChannelName());
    }
    channel->feed(event);
    event.releaseBufferList();
}

void Worker::handleListenDeregister(ProceededEvent &event) const {
    auto channel = this->channelDb->getChannel(event.getChannelName());
    if (channel != nullptr) {
        channel->feed(event);
    }
    event.releaseBufferList();
}

void Worker::handleListenDeregisterAll(ProceededEvent &event) const {
    auto channelVec = this->channelDb->getAllListenChannelsForId(event.getSender_id());
    for (const auto &channel : *channelVec) {
        channel->feed(event);
    }
    event.releaseBufferList();
}

void Worker::handleListenDisconnect(ProceededEvent &event) const {
    handleListenDeregisterAll(event);
    consumerServer->disconnectClient(event.getSender_id());
}

void Worker::handleListenListChannels(ProceededEvent &event) const {
    string str = this->channelDb->getChannelListStr();
    auto buff = BufferPool::bufferPool->getChunkedBuffer(LISTEN_LIST_CHANELES_RES,
                                                         str.c_str(),
                                                         str.size());
    if (buff != nullptr) {
        this->consumerServer->send(make_shared<TcpServerOutcomeMessage>(buff, event.getSender_id()));
    } else{
        LOG_ERROR("got null buff");
    }
    event.releaseBufferList();
}

// setters
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
