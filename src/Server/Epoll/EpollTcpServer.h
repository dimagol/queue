#include <unistd.h>
#include <string>
#include <pthread.h>
#include <unordered_map>
#include <sys/epoll.h>
#include "../../Buff/BufferPool.h"
#include "../../Queue/ConcurrentQueue.h"
#include "../../Logging/TSLogger.h"
#include "../../Buff/ThreadSafeBufferList.h"
#include "ThreadSafeUserDataMap.h"
#include "UserData.h"
#include "../../Queue/ConcurrentQueueSingleConsumer.h"
#include "MsgFromClient.h"

#define LISTENQ     20
#define TIMEOUT     500

#ifndef EPOL_TCP_SERVER_H
#define EPOL_TCP_SERVER_H

class EpollTcpServer{

    class IntHolder{
    public:
        explicit IntHolder(int val) : val(val) {}
        int val;
    };


private:
    ThreadSafeUserDataMap userDataMap;

    int ePollFd;
    epoll_event listenerEpollEvent;
    epoll_event events[LISTENQ];
    int listenfd;
    volatile bool shouldRun = true;
    ConcurrentQueue<shared_ptr<IntHolder>> readQue;
    ConcurrentQueue<shared_ptr<IntHolder>> writeQue;
    ConcurrentQueue<MsgFromClient *> outQue;
    uint16_t listenPort;
    std::string listenAddr;
    std::thread *writeTaskThread;
    std::thread *readTaskThread;


    void handleRead(const epoll_event *event);

    void handleWrite(const epoll_event *event);

    void initRWThreads();

    bool listenOnEpoll();

    void * readTask();

    void * writeTask();

    bool setNonBlocking(int sock);

    bool runServer();

    void acceptClient(const epoll_event *event);

    void handleIncomeMsg(ThreadSafeBufferList * list, uint32_t clientId);

    void startRecv(int fd) const;

    void startSend(int fd) const;

    int readNBytes(int fd, uint8_t *buffer, uint32_t len);

    int writeNBytes(int fd, uint8_t *buffer, uint32_t len);

public:

    void setShouldRun(bool shouldRun);

    bool run();

    void disconnectClient(int fd);

    void sendToClient(int fd, ThreadSafeBufferList * list);

    void init(string &addr, uint16_t port);

    MsgFromClient * tryRecieve();

    MsgFromClient * recieve();

};


#endif