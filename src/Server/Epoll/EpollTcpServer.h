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

#define LISTENQ     20
#define TIMEOUT     500

class EpollTcpServer{

    class EpollTask {
    public:
        explicit EpollTask(epoll_data_t data){
            this->data = data;
        }
        epoll_data_t data{0};
    };

    ThreadSafeUserDataMap userDataMap;

    int ePollFd;
    epoll_event listenerEpollEvent;
    epoll_event events[LISTENQ];
    int listenfd;
    volatile bool shouldRun = true;
    ConcurrentQueue<EpollTask*> readQue;
    ConcurrentQueue<EpollTask*> writeQue;
    uint16_t listenPort;
    std::string listenAddr;
    std::thread *writeTaskThread;
    std::thread *readTaskThread;



    void handleRead(const epoll_event *event);

    void handleWrite(const epoll_event *event);

    void initRWThreads();

    void listenOnEpoll();

    void * readTask();

    void * writeTask();

    void setNonBlocking(int sock);

    int runServer();

    void acceptClient(const epoll_event *event);

    void handleIncomeMsg(ThreadSafeBufferList * list);

public:
    void test();

    int test1();

    void shutdown();

    int readNBytes(int fd, uint8_t *buffer, uint32_t len);

    void disconnectClient(int fd);

    int writeNBytes(int fd, uint8_t *buffer, uint32_t len);

    void sendToClient(int fd, ThreadSafeBufferList * list);

    void startRecv(int fd) const;

    void startSend(int fd) const;
};


