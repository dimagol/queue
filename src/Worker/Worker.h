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
    void run();

    // setters
    void setChannelDb(const shared_ptr<ChannelDb> &channelDb);

    void setProducerServer(const shared_ptr<TcpServer> &producerServer);

    void setConsumerServer(const shared_ptr<TcpServer> &consumerServer);

    void setShouldRun(volatile bool shouldRun);

private:
    // post handlers
    inline void handlePostMsg(ProceededEvent &event) const;

    inline void handlePostRegister(ProceededEvent &event) const;

    inline void handlePostDeregister(ProceededEvent &event) const;

    inline void handlePostDeregisterAll(ProceededEvent &event) const;

    inline void handlePostDisconnect(ProceededEvent &event) const;

    inline void handlePostListChannels(ProceededEvent &event) const;

    inline void handlePost(ProceededEvent &event) const;


    // listen handlers
    inline void handleListenMsg(ProceededEvent &event) const;

    inline void handleListenRegister(ProceededEvent &event) const;

    inline void handleListenDeregister(ProceededEvent &event) const;

    inline void handleListenDeregisterAll(ProceededEvent &event) const;

    inline void handleListenDisconnect(ProceededEvent &event) const;

    inline void handleListenListChannels(ProceededEvent &event) const;


private:
    volatile bool shouldRun = true;
    shared_ptr<ChannelDb> channelDb;
    shared_ptr<TcpServer> producerServer;
    shared_ptr<TcpServer> consumerServer;
    MsgToEventProcessor processor;

};


#endif //TCP_SHMAFKA_WORKER_H
