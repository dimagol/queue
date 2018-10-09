//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_WORKER_H
#define TCP_SHMAFKA_WORKER_H


#include "../ChanelDb/ChannelDb.h"
#include "../Processor/MsgToEventProcessor.h"
#include "../Msg/MsgBuilder.h"
#include "../Server/Epoll/EpollTcpServer.h"

class Worker {

public:
    void run();

    // setters
    void setChannelDb(ChannelDb * channelDb);

    void setProducerServer(EpollTcpServer* producerServer);

    void setConsumerServer(EpollTcpServer* consumerServer);

    void setShouldRun(volatile bool shouldRun);

    void setWaitingStrategy(WaitingStrategy *waitingStrategy);

    void setBuilder(MsgBuilder *builder);

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
    ChannelDb * channelDb;
    EpollTcpServer * producerServer;
    EpollTcpServer * consumerServer;
    MsgToEventProcessor processor;
    MsgBuilder  *msgBuilder;
    WaitingStrategy *waitingStrategy;




};


#endif //TCP_SHMAFKA_WORKER_H
