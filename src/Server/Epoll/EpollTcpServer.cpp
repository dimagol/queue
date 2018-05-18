//#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "EpollTcpServer.h"
#include "SimpleMsgHeader.h"
#include <errno.h>

// maximum received data byte
void EpollTcpServer::setNonBlocking(int sock) {
    int opts;
    if ((opts = fcntl(sock, F_GETFL)) < 0) {
        LOG_ERROR("GETFL failed for", sock);
        exit(-1);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        LOG_ERROR("SETFL failed for", sock);
        exit(-1);
    }
}

int EpollTcpServer::runServer() {
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
            }
        }
    }

    return 0;
}

void EpollTcpServer::listenOnEpoll() {
    struct sockaddr_in serveraddr{0};
    // epoll descriptor, for handling accept
    ePollFd = epoll_create(256);
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // set the descriptor as non-blocking
    setNonBlocking(listenfd);
    // event related descriptor
    listenerEpollEvent.data.fd = listenfd;
    // monitor in message, edge trigger
    listenerEpollEvent.events = EPOLLIN | EPOLLET;
    // register epoll event
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, listenfd, &listenerEpollEvent);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(listenPort);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
}



int EpollTcpServer::test1()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 8082 );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
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
    writeQue.push((EpollTask *)&event->data.fd);

}

void EpollTcpServer::handleRead(const epoll_event *event) {
    if (event->data.fd < 0){
        LOG_ERROR("got bad fd")
    } else {
        readQue.push((EpollTask *)&event->data.fd);
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
        EpollTask* taskPtr = readQue.pop();
        fd = taskPtr->data.fd;
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
        if(recvBuffer->offset < MSG_LEN_BUFF_LEN) {
            int ret = readNBytes(fd,
                                 recvBuffer->msg_len_buff + recvBuffer->offset,
                                 recvBuffer->offset - MSG_LEN_BUFF_LEN);
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
                                 recvBuffer->msg_data_buff + recvBuffer->offset,
                                 recvBuffer->get_msg_all_data_len() - recvBuffer->offset);

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

        if(recvBuffer->offset == recvBuffer->get_msg_all_data_len()) {
            LOG_INFO("got msg from fd ", fd, " len ", recvBuffer->get_msg_all_data_len())
            uint32_t numOfChunks = recvBuffer->get_int(SIMPLE_MSG_CHUNK_NUMBER_OF_CHUNCKS_OFFSET);
            uint32_t chunkNum = recvBuffer->get_int(SIMPLE_MSG_CHUNK_NUMBER_OFFSET);
            if (numOfChunks > chunkNum){
                userDataPtr->recvList->append(BufferPool::bufferPool->get());
            } else if (numOfChunks == chunkNum) {
                handleIncomeMsg(userDataPtr->recvList);
                userDataPtr->recvList = new ThreadSafeBufferList();
            } else if (numOfChunks < chunkNum){
                LOG_ERROR("numOfChunks < chunkNum ", numOfChunks , " ", chunkNum)
                disconnectClient(fd);
                continue;
            }
            goto READ_HEADER;
        }
    }
}

void EpollTcpServer::sendToClient(int fd, ThreadSafeBufferList * list){
    UserData * userDataPtr = userDataMap.getFromMap((uint32_t)fd);

    if(userDataPtr == nullptr) {
        LOG_ERROR("cant find client for fd", fd)
        // todo relese
        return;
    }

    userDataPtr->sendListLsits->append(list);
    startSend(fd);
}

void EpollTcpServer::startSend(int fd) const {
    epoll_event event{0};
    event.events = EPOLLOUT | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &event);
}


void *EpollTcpServer::writeTask() {
    int fd;
    ssize_t n;
    while(shouldRun)
    {
        EpollTask * taskPtr = writeQue.pop();
        fd = taskPtr->data.fd;
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

        uint32_t lenSend = out->get_msg_all_data_len() - out->offset;
        int ret = writeNBytes(fd, out->msg_len_buff + out->offset, lenSend);
        if(ret == -1){
            //todo release all
            LOG_ERROR("cant send data to fd ", fd)
            disconnectClient(fd);
            continue;
        } else if (lenSend == ret){
            LOG_INFO("buffer was sent to ", fd)
            // release buffer
            BufferPool::bufferPool->releaseOne(userDataPtr->sendListLsits->get_head()->remove_head());
            // if next buffer is null
            if (userDataPtr->sendListLsits->get_head()->get_head() == nullptr){
                // remove list
                delete(userDataPtr->sendListLsits->remove_head());
                if (userDataPtr->sendListLsits->get_head() == nullptr){
                    // if next list is null cont to listen
                    // todo raise
                    startRecv(fd);
                    if (userDataPtr->sendListLsits->get_head() != nullptr){
                        // raise mgmt
                        startSend(fd);
                    }
                }
            }

        } else {
            out->offset += ret;
        }
    }

}

void EpollTcpServer::startRecv(int fd) const {
    epoll_event event{0};
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &event);
}

void EpollTcpServer::test() {
    this->listenAddr = "127.0.0.0";
    this->listenPort = 8082;
    listenOnEpoll();
    initRWThreads();
    runServer();


}

void EpollTcpServer::handleIncomeMsg(ThreadSafeBufferList * list) {

}

void EpollTcpServer::shutdown() {
    shouldRun = false;

}


