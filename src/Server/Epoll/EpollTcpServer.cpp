#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "EpollTcpServer.h"
#include "../../Logging/TSLogger.h"

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
    inet_aton(listenAddr.c_str(), &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(listenPort);
    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
}

void EpollTcpServer::initRWThreads() const {
    // threads for reading thread pool
    pthread_create(&tid1, NULL, readTask, NULL);
    // threads for respond to client
    pthread_create(&tid2, NULL, writeTask, NULL);
}

void EpollTcpServer::handleWrite(const epoll_event *event) {
    if (event->data.ptr == nullptr){
        LOG_ERROR("no data ptr for event")
        return;
    }
    shared_ptr<EpollTask> ptr = make_shared(event->data);
    writeQue.push(ptr);

}

void EpollTcpServer::handleRead(const epoll_event *event) {
    if (event->data.fd < 0){
        LOG_ERROR("got bad fd")
    } else {
        shared_ptr<EpollTask> ptr = make_shared(event->data);
        readQue.push(ptr);
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

    shared_ptr<UserData> ptr = make_shared(UserData());
    putToMap((uint32_t)connfd, ptr);
    LOG_INFO("new client added " , connfd)
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, connfd, &listenerEpollEvent);
}


void *EpollTcpServer::readTask(void *args) {
    int fd;
    ssize_t n;
    while(shouldRun)
    {
        shared_ptr<EpollTask> taskPtr = readQue.pop();
        fd = taskPtr->data.fd;
        if(fd < 0){
            LOG_ERROR("got bad fd")
            continue;
        }
        shared_ptr<UserData> userDataPtr = getFromMap((uint32_t)fd);

        if(userDataPtr == nullptr){
            LOG_ERROR("no client for fd ", fd);
            continue;
        }

        if(userDataPtr->bufferIn == nullptr){
            userDataPtr->bufferIn = BufferPool::bufferPool->get();
        }

        if(userDataPtr->bufferIn->offset < MSG_LEN_BUFF_LEN) {
            n = recv(fd, userDataPtr->bufferIn->msg_len_buff, MSG_LEN_BUFF_LEN - userDataPtr->bufferIn->offset, 0);
        } else {
            uint32_t msgLen = userDataPtr->bufferIn->get_msg_len();
            if(msgLen > userDataPtr->bufferIn->len){
                LOG_ERROR("msg msgLen bigger than buffer")
                removeFromMap((uint32_t)fd);
                close(fd);
                continue;
            }
            n = recv(fd, userDataPtr->bufferIn->msg_len_buff, msgLen - userDataPtr->bufferIn->offset, 0);
        }


        if(n < 0){
            LOG_ERROR("got error while recv for fd ", fd)
            removeFromMap((uint32_t)fd);
            close(fd);
            continue;
        }

        if(n == 0){
            LOG_ERROR("client disconnected ", fd)
            removeFromMap((uint32_t)fd);
            close(fd);
            continue;
        }

        userDataPtr->bufferIn->offset += n;
        if (userDataPtr->bufferIn->offset == userDataPtr->bufferIn->get_msg_all_data_len()){
            handleIncomeMsg(userDataPtr->bufferIn);
            userDataPtr->bufferIn = BufferPool::bufferPool->get();
        }

//        if(n < )
//        //echo("[SERVER] thread %d readTask before lock\n", pthread_self());
//        // protect EpollTask queue (readhead/readtail)
//        pthread_mutex_lock(&r_mutex);
//        //echo("[SERVER] thread %d readTask after lock\n", pthread_self());
//        while(readhead == NULL)
//            // if condl false, will unlock mutex
//            pthread_cond_wait(&r_condl, &r_mutex);
//
//        fd = readhead->data.fd;
//        struct EpollTask* tmp = readhead;
//        readhead = readhead->next;
//        free(tmp);
//
//        //echo("[SERVER] thread %d readTask before unlock\n", pthread_self());
//        pthread_mutex_unlock(&r_mutex);
//        //echo("[SERVER] thread %d readTask after unlock\n", pthread_self());
//
////        echo("[SERVER] readTask %d handling %d\n", pthread_self(), fd);
//        data = malloc(sizeof(struct UserData));
//        data->fd = fd;
//        if ((n = recv(fd, data->line, MAXBTYE, 0)) < 0)
//        {
//            if (errno == ECONNRESET)
//                close(fd);
////            echo("[SERVER] Error: readline failed: %s\n", strerror(errno));
//            if (data != NULL)
//                free(data);
//        }
//        else if (n == 0)
//        {
//            close(fd);
////            echo("[SERVER] Error: client closed connection.\n");
//            if (data != NULL)
//                free(data);
//        }
//        else
//        {
//            data->n_size = n;
//            for (i = 0; i < n; ++i)
//            {
//                if (data->line[i] == '\n' || data->line[i] > 128)
//                {
//                    data->line[i] = '\0';
//                    data->n_size = i + 1;
//                }
//            }
////            echo("[SERVER] readTask %d received %d : [%d] %s\n", pthread_self(), fd, data->n_size, data->line);
//            if (data->line[0] != '\0')
//            {
//                // modify monitored event to EPOLLOUT,  wait next loop to send respond
//                listenerEpollEvent.data.ptr = data;
//                // Modify event to EPOLLOUT
//                listenerEpollEvent.events = EPOLLOUT | EPOLLET;
//                // modify moditored fd event
//                epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &listenerEpollEvent);
//            }
//        }
    }
}

void *EpollTcpServer::writeTask(void *args) {
    int fd;
    ssize_t n;
    while(shouldRun)
    {
        shared_ptr<EpollTask> taskPtr = writeQue.pop();
        fd = taskPtr->data.fd;
        shared_ptr<UserData> userDataPtr = getFromMap((uint32_t)fd);

        if(userDataPtr == nullptr) {
            LOG_ERROR("client is not exist ", fd)
            close(fd);
            continue;
        }

        SocketProtoBuffer * out = userDataPtr->bufferOut;
        if (out == nullptr){
            LOG_ERROR("no buffer found for ", fd)
            removeFromMap((uint32_t)fd);
            close(fd);
            continue;
        }

        n = send(fd, out->msg_len_buff, out->get_msg_all_data_len() - out->offset, 0);

        if(n < 0){
            LOG_ERROR("got error while send for fd ", fd)
            removeFromMap((uint32_t)fd);
            close(fd);
            continue;
        }

        if(n == 0){
            LOG_ERROR("client disconnected ", fd)
            removeFromMap((uint32_t)fd);
            close(fd);
            continue;
        }

        out->offset += n;

        if(out->offset == out->get_msg_all_data_len()){
            LOG_INFO("sent complete msg")
            BufferPool::bufferPool->releaseOne(out);
        }

        // modify monitored event to EPOLLOUT,  wait next loop to send respond
        listenerEpollEvent.data.ptr = data;
        // Modify event to EPOLLOUT
        listenerEpollEvent.events = EPOLLOUT | EPOLLET;
        // modify moditored fd event
        epoll_ctl(ePollFd, EPOLL_CTL_MOD, fd, &listenerEpollEvent)

        rdata = (struct UserData*)epollTask.data.ptr;
        struct EpollTask* tmp = writehead;
        writehead = writehead->next;
        free(tmp);

        //echo("[SERVER] thread %d writeTask before unlock\n", pthread_self());
        pthread_mutex_unlock(&w_mutex);
        //echo("[SERVER] thread %d writeTask after unlock\n", pthread_self());

//        echo("[SERVER] writeTask %d sending %d : [%d] %s\n", pthread_self(), rdata->fd, rdata->n_size, rdata->line);
        // send responce to client
        if ((n = send(rdata->fd, rdata->line, rdata->n_size, 0)) < 0)
        {
            if (errno == ECONNRESET)
                close(rdata->fd);
            echo("[SERVER] Error: send responce failed: %s\n", strerror(errno));
        }
        else if (n == 0)
        {
            close(rdata->fd);
//            echo("[SERVER] Error: client closed connection.");
        }
        else
        {
            // modify monitored event to EPOLLIN, wait next loop to receive data
            listenerEpollEvent.data.fd = rdata->fd;
            // monitor in message, edge trigger
            listenerEpollEvent.events = EPOLLIN | EPOLLET;
            // modify moditored fd event
            epoll_ctl(ePollFd, EPOLL_CTL_MOD, rdata->fd, &listenerEpollEvent);
        }
        // delete data
        free(rdata);
    }
}

void EpollTcpServer::handleIncomeMsg(SocketProtoBuffer *pBuffer) {

}


