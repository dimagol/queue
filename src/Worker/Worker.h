//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_WORKER_H
#define TCP_SHMAFKA_WORKER_H


#include "../ChanelDb/ChannelDb.h"
#include "../Server/TcpServer.h"
#include "../Processor/MsgToEventProcessor.h"

class Worker {

public:
    void run(){
        if (channelDb == nullptr || producerServer == nullptr || consumerServer == nullptr ){
            LOG_ERROR("not set");
            return;
        }
        while (shouldRun){
            auto prodMsg = producerServer->recieve();
            if(prodMsg != nullptr){
                auto event = processor.processBuff(*prodMsg);

                switch (event.getType()){
                    case MsgType::POST_REGISTER:
                        handlePostRegister(event);
                        break;
                    case MsgType::POST:
                        handlePost(event);
                        break;
                    case MsgType::POST_DEREGISTER:
                        handlePostDeregister(event);
                        break;
                    case MsgType::POST_DEREGISTER_ALL:
                        handlePostDeregisterAll(event);
                        break;
                    case MsgType::POST_DISCONNECT:
                        handlePostDeregisterAll(event);
                        producerServer->disconnectClient(event.getSender_id());
                        break;
                    case MsgType::LIST_CHANELES:
                        handlePostListChannels(event);
                        break;
                    default:
                        LOG_ERROR("");
                }
            }
            auto consMsg = consumerServer->recieve();
            if(consMsg != nullptr){
                auto event = processor.processBuff(*prodMsg);
                switch (event.getType()){
                    case MsgType::LISTEN_REGISTER:
                    case MsgType::LISTEN_DEREGISTER:
                    case MsgType::LISTEN_DEREGISTER_ALL:
                    case MsgType::LISTEN_DISCONNECT:
                    case MsgType::LIST_CHANELES:
                        break;
                    default:
                        LOG_ERROR("");
                }
            }

        }
    }

    void handlePostListChannels(ProceededEvent &event) const {
                        string str = this->channelDb->getChannelListStr();
                        auto buff = BufferPool::bufferPool->getChunkedBuffer(LIST_CHANELES, str.c_str(),
                                                                             str.size());
                        if (buff != nullptr) {
                            this->producerServer->send(make_shared<TcpServerOutcomeMessage>(buff, event.getSender_id()));
                        } else{
                            LOG_ERROR("got null buff");
                        }
                        event.releaseBufferList();
    }

    void handlePostDeregisterAll(ProceededEvent &event) const {
                        auto channelVec = this->channelDb->getAllPostChannelsForId(event.getSender_id());
                        for (auto channel : *channelVec) {
                            channel->feed(event);
                        }
                        event.releaseBufferList();
    }

    void handlePostDeregister(ProceededEvent &event) const {
                        auto channel = this->channelDb->getChannel(event.getChannelName());
                        if (channel != nullptr) {
                            channel->feed(event);
                        }
                        event.releaseBufferList();
    }

    void handlePost(ProceededEvent &event) const {
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

    void handlePostRegister(ProceededEvent &event) const {
        auto channel = channelDb->getChannel(event.getChannelName());
        if (channel == nullptr){
            channel = channelDb->createChannel(event.getChannelName());
        }
        event.releaseBufferList();
    }

    void setChannelDb(const shared_ptr<ChannelDb> &channelDb);

    void setProducerServer(const shared_ptr<TcpServer> &producerServer);

    void setConsumerServer(const shared_ptr<TcpServer> &consumerServer);

    void setShouldRun(volatile bool shouldRun);

private:
    volatile bool shouldRun = true;

    shared_ptr<ChannelDb> channelDb;
    shared_ptr<TcpServer> producerServer;
    shared_ptr<TcpServer> consumerServer;

    MsgToEventProcessor processor;

};


#endif //TCP_SHMAFKA_WORKER_H
