//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_SERVERPOSTTHREAD_H
#define TCP_SHMAFKA_SERVERPOSTTHREAD_H

#include <thread>
#include <boost/asio/io_service.hpp>
#include "../Logging/TSLogger.h"
#include "../Server/Epoll/EpollTcpServer.h"

using namespace std;
class ServerThread {
public:
    void init(EpollTcpServer *server_ptr){
        LOG_INFO("ServerThread initiate")
        server = server_ptr;

    }

    void run(){
        LOG_INFO("ServerThread run")
        serverThread = new thread(&EpollTcpServer::run, server);
    }

    void join(){
        serverThread->join();
    }
    virtual ~ServerThread() {
        LOG_INFO("ServerThread end")
        server->setShouldRun(false);
        serverThread->join();
        delete serverThread;
    }


private:
    EpollTcpServer *server = nullptr;
    std::thread *serverThread = nullptr;
};


#endif //TCP_SHMAFKA_SERVERPOSTTHREAD_H
