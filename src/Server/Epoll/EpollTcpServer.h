#include <unistd.h>
#include <string>
#include <pthread.h>
#include <unordered_map>
#include "../../Buff/BufferPool.h"
#include "../../Queue/ConcurrentQueue.h"
#include "../../Logging/TSLogger.h"

#define LISTENQ     20
#define TIMEOUT     500

class EpollTcpServer{

    class UserData {
    public:
        int fd = -1;
        SocketProtoBuffer* bufferOut = nullptr;
        SocketProtoBuffer* bufferIn = nullptr;
    };

    class EpollTask {
    public:
        explicit EpollTask(epoll_data_t data){
            this->data = data;
        }
        epoll_data_t data{0};
    };


    unordered_map<uint32_t , shared_ptr<UserData>> clientMap;

    mutex mapLock;

    shared_ptr<UserData> getFromMap(uint32_t id){
        mapLock.lock();
        auto fromMap = clientMap.find(id);
        auto mapEnd = clientMap.end();
        mapLock.unlock();
        if(fromMap == mapEnd){
            return nullptr;
        } else{
            return fromMap->second;
        }
    }

    bool putToMap(uint32_t id, shared_ptr<UserData>  ptr){
        mapLock.lock();
        bool isInMap = clientMap.find(id) == clientMap.end();
        if(!isInMap){
            clientMap.insert(make_pair(id,ptr));
        } else{
            LOG_ERROR("id ", id, " already in map")
        }
        mapLock.unlock();
        return !isInMap;
    }

    bool removeFromMap(uint32_t id){
        mapLock.lock();
        bool isInMap = clientMap.find(id) == clientMap.end();
        if(isInMap){
            clientMap.erase(id);
            LOG_INFO("client ", id, " was removed")
        } else{
            LOG_ERROR("id ", id, " not in map")
        }
        mapLock.unlock();
        return isInMap;
    }

    bool mapContains(uint32_t id){
        mapLock.lock();
        bool isInMap = clientMap.find(id) != clientMap.end();
        mapLock.unlock();
        return isInMap;
    }

    int ePollFd;
    epoll_event listenerEpollEvent;
    epoll_event events[LISTENQ];
    int listenfd;
    volatile bool shouldRun = true;
    ConcurrentQueue<shared_ptr<EpollTask>> readQue;
    ConcurrentQueue<shared_ptr<EpollTask>> writeQue;
    uint16_t listenPort;
    std::string listenAddr;
    pthread_t tid1;
    pthread_t tid2;



    void handleRead(const epoll_event *event);

    void handleWrite(const epoll_event *event);

    void initRWThreads() const;

    void listenOnEpoll();

    void * readTask(void *args);

    void * writeTask(void *args);

    void setNonBlocking(int sock);

    int runServer();

    void acceptClient(const epoll_event *event);

    void handleIncomeMsg(SocketProtoBuffer *pBuffer);
};


