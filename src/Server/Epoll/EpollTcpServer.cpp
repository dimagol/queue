#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "EpollTcpServer.h"
#include "SimpleMsgHeader.h"

// maximum received data byte
bool EpollTcpServer::setNonBlocking(int sock) {
    int opts;
    if ((opts = fcntl(sock, F_GETFL)) < 0) {
        LOG_ERROR("GETFL failed for", sock);
        return false;
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        LOG_ERROR("SETFL failed for", sock);
        return false;
    }
    return true;
}

bool EpollTcpServer::runServer() {
    while (shouldRun)
    {
        int nfds = epoll_wait(ePollFd, events, LISTENQ, TIMEOUT);
        for (int eventId = 0; eventId < nfds; ++eventId)
        {
            // Get new connection
            epoll_event * event = &events[eventId];
            if (event->data.fd == listenfd){
                acceptClient(event);
            } else if ((event->events & EPOLLIN) != 0u){
                handleRead(event);
            } else if ((event->events & EPOLLOUT) != 0u){
                handleWrite(event);
            } else{
                LOG_ERROR("unknown epoll event");
                return false;
            }
        }
    }

    return true;
}

bool EpollTcpServer::listenOnEpoll() {
    struct sockaddr_in serveraddr{0};
    // epoll descriptor, for handling accept
    ePollFd = epoll_create(256);
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // set the descriptor as non-blocking
    if (!setNonBlocking(listenfd)){
        return false;
    }
    // event related descriptor
    listenerEpollEvent.data.fd = listenfd;
    // monitor in message, edge trigger
    listenerEpollEvent.events = EPOLLIN | EPOLLET;
    // register epoll event
    if(epoll_ctl(ePollFd, EPOLL_CTL_ADD, listenfd, &listenerEpollEvent) != 0){
        LOG_ERROR("error while running epol ctl")
        return false;
    };

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(listenPort);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0){
        LOG_ERROR("error while running bind")
        return false;
    };

    if(listen(listenfd, LISTENQ) != 0){
        LOG_ERROR("error while running listen")
        return false;
    };

    return true;

}

void EpollTcpServer::initRWThreads() {
    // threads for reading thread pool
    writeTaskThread = new thread(&EpollTcpServer::writeTask, this);
    readTaskThread = new thread(&EpollTcpServer::readTask, this);
}

void EpollTcpServer::handleWrite(const epoll_event *event) {
    if (event->data.fd < 0){
        LOG_ERROR("no data ptr for event")
        return;
    }

    writeQue.push(make_shared<IntHolder>((event->data.fd)));

}

void EpollTcpServer::handleRead(const epoll_event *event) {
    if (event->data.fd < 0){
        LOG_ERROR("got bad fd")
    } else {
        readQue.push(make_shared<IntHolder>((event->data.fd)));
    }
}

void EpollTcpServer::acceptClient(const epoll_event *event) {// accept the client connection
    sockaddr_in clientaddr{0};
    socklen_t clilentLen;
    int connfd = accept(event->data.fd, (struct sockaddr*)&clientaddr, &clilentLen);
    if (connfd < 0){
        LOG_ERROR("unable to accept connection")
        return;
    }
    setNonBlocking(connfd);
    listenerEpollEvent.data.fd = connfd;
    // monitor in message, edge trigger
    listenerEpollEvent.events = EPOLLIN | EPOLLET;
    // add fd to epoll queue

    UserData * ptr = new UserData();
    userDataMap.putToMap((uint32_t)connfd, ptr);
    LOG_INFO("new client added " , connfd)
    // register for read from client
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, connfd, &listenerEpollEvent);
}

int EpollTcpServer::readNBytes(int fd, uint8_t * buffer, uint32_t len){
    ssize_t n = recv(fd, buffer, len, 0);
    if (n == -1){
        if (errno != EAGAIN){
            // error while reading
            LOG_ERROR("error while reading fd ", fd)
            return -1;
        } else {
            LOG_INFO("done reading fd ", fd)
            return 0;
        }
    } else if (n == 0){
        LOG_INFO("EOF remote closed fd ", fd)
        return -1;
    } else {
        LOG_INFO("read ", n ," data fd", fd)
        return (int)n;
    }
}

int EpollTcpServer::writeNBytes(int fd, uint8_t * buffer, uint32_t len){
    ssize_t n = send(fd, buffer, len, 0);
    if (n == -1){
        if (errno != EAGAIN){
            // error while reading
            LOG_ERROR("error while reading fd ", fd)
            return -1;
        } else {
            LOG_INFO("done writing fd ", fd)
            return 0;
        }
    } else if (n == 0){
        LOG_INFO("cant write fd ", fd)
        return 0;
    } else {
        LOG_INFO("wrote ", n ," data fd", fd)
        return (int)n;
    }
}

void EpollTcpServer::disconnectClient(int fd){
    userDataMap.removeFromMap(fd);
    struct epoll_event event;
    epoll_ctl(ePollFd, EPOLL_CTL_DEL, fd, &event);
    close(fd);
    LOG_INFO(fd, " was removed")
}

void *EpollTcpServer::readTask() {

    // todo handle bunch of errors with buffer len and reorder and null
    int fd;
    while(shouldRun)
    {
        fd = readQue.pop()->val;
        if(fd < 0){
            LOG_ERROR("got bad fd")
            continue;
        }
        UserData * userDataPtr = userDataMap.getFromMap((uint32_t)fd);

        if(userDataPtr == nullptr){
            disconnectClient(fd);
            LOG_ERROR("no client for fd ", fd);
            continue;
        }

        SocketProtoBuffer * recvBuffer = userDataPtr->recvList->get_tail();
        if(recvBuffer == nullptr){
            // todo handle null
            recvBuffer = BufferPool::bufferPool->get();
            userDataPtr->recvList->append(recvBuffer);
        }


        READ_HEADER:
        if(recvBuffer->offset < SIMPLE_MSG_LEN_LEN) {
            int ret = readNBytes(fd,
                                 recvBuffer->msg_complete_buff + recvBuffer->offset,
                                 SIMPLE_MSG_LEN_LEN - recvBuffer->offset);

            if (ret == -1){
                disconnectClient(fd);
                continue;
            } else if (ret > 0){
                recvBuffer->offset += ret;
            } else if (ret == 0) {
                // nothing to read
                continue;
            } else{
                disconnectClient(fd);
                LOG_ERROR("unsupported ret ", ret)
                continue;
            }
        }

        if(recvBuffer->offset >= MSG_LEN_BUFF_LEN) {
            int ret = readNBytes(fd,
                                 recvBuffer->msg_complete_buff + recvBuffer->offset,
                                 recvBuffer->get_msg_len() - recvBuffer->offset);

            if (ret == -1){
                disconnectClient(fd);
                continue;
            } else if (ret > 0){
                recvBuffer->offset += ret;
            } else if (ret == 0) {
                // nothing to read
                continue;
            } else{
                disconnectClient(fd);
                LOG_ERROR("unsupported ret ", ret)
                continue;
            }
        }

        if(recvBuffer->offset == recvBuffer->get_msg_len()) {
            LOG_INFO("got msg from fd ", fd, " len ", recvBuffer->get_msg_len())
            uint32_t numOfChunks = recvBuffer->get_int(SIMPLE_MSG_NUMBER_OF_CHUNCKS_OFFSET);
            uint32_t chunkNum = recvBuffer->get_int(SIMPLE_MSG_CHUNK_NUMBER_OFFSET);
            if (numOfChunks > chunkNum){
                userDataPtr->recvList->append(BufferPool::bufferPool->get());
                recvBuffer = userDataPtr->recvList->get_tail();
            } else if (numOfChunks == chunkNum) {
                handleIncomeMsg(userDataPtr->recvList, fd);
                userDataPtr->recvList = new ThreadSafeBufferList();
            } else if (numOfChunks < chunkNum){
                LOG_ERROR("numOfChunks < chunkNum ", numOfChunks , " ", chunkNum)
                disconnectClient(fd);
                continue;
            }
            goto READ_HEADER;
        }
    }
    return nullptr;
}

void EpollTcpServer::sendToClient(int fd, ThreadSafeBufferList * list){
    UserData * userDataPtr = userDataMap.getFromMap((uint32_t)fd);

    if(userDataPtr == nullptr) {
        LOG_ERROR("cant find client for fd", fd)
        return;
    }

    userDataPtr->sendListLsits->append(list);
    startSend(fd);
}

void EpollTcpServer::startSend(int fd) const {
    epoll_event event{0};
    event.events = EPOLLOUT | EPOLLET | EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &event);
}


void *EpollTcpServer::writeTask() {
    int fd;
    while(shouldRun)
    {
        fd = writeQue.pop()->val;
        UserData * userDataPtr = userDataMap.getFromMap((uint32_t)fd);

        if(userDataPtr == nullptr) {
            disconnectClient(fd);
            LOG_ERROR("client is not exist ", fd)
            continue;
        }

        SocketProtoBuffer * out = userDataPtr->sendListLsits->get_head()->get_head();
        if (out == nullptr){
            LOG_ERROR("no buffer found for ", fd)
            disconnectClient(fd);
            continue;
        }

        uint32_t lenSend = out->get_msg_len() - out->offset;
        int ret = writeNBytes(fd, out->msg_complete_buff+ out->offset, lenSend);
        if(ret == -1){
            //todo releaseAllChain all
            LOG_ERROR("cant send data to fd ", fd)
            disconnectClient(fd);
            continue;
        } else if (lenSend == ret){
            LOG_INFO("buffer was sent to ", fd)

            // if next buffer is null
            if (userDataPtr->sendListLsits->get_head()->get_head() == nullptr){
                // remove list
                delete(userDataPtr->sendListLsits->remove_head());
                if (userDataPtr->sendListLsits->get_head() == nullptr){
                    // if next list is null cont to listen
                    startRecv(fd);
                    if (userDataPtr->sendListLsits->get_head() != nullptr){
                        startSend(fd);
                    }
                }
                else {
                    startSend(fd);
                }
            } else {
                startSend(fd);
            }
        } else {
            out->offset += ret;
            startSend(fd);
        }
    }
    return nullptr;
}

void EpollTcpServer::startRecv(int fd) const {
    epoll_event event{0};
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &event);
}

bool EpollTcpServer::run() {
    if (!listenOnEpoll()){
        LOG_ERROR("cant listen to epol")
        return false;
    }
    initRWThreads();
    runServer();
    return true;
}

void EpollTcpServer::init(string &addr, uint16_t port){
    this->listenAddr = addr;
    this->listenPort = port;
}

void EpollTcpServer::handleIncomeMsg(ThreadSafeBufferList * list, uint32_t clientId) {
    auto to_print = list->get_head();
    while (to_print != nullptr){
        to_print->offset = 0;
        to_print = to_print->nextBuffer;
    }

    outQue.push(new MsgFromClient(list,clientId));

}

void EpollTcpServer::setShouldRun(bool shouldRun) {
    this->shouldRun = shouldRun;
}

 MsgFromClient * EpollTcpServer::tryRecieve() {
    return outQue.try_pop();
}

MsgFromClient *EpollTcpServer::recieve() {
    return outQue.pop();
}


