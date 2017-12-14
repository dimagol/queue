//
// Created by dima on 07/12/17.
//

#ifndef TCP_SHMAFKA_SERVERPOSTTHREAD_H
#define TCP_SHMAFKA_SERVERPOSTTHREAD_H

#include <thread>
#include <boost/asio/io_service.hpp>
#include "../Server/TcpServer.h"
#include "../Logging/TSLogger.h"

using namespace std;
class ServerThread {
public:
    void init(TcpServer *server_ptr){
        LOG_INFO("ServerThread initiate")
        server = server_ptr;
    }

    void run(){
        LOG_INFO("ServerThread run")
        serverThread = new thread(&TcpServer::run, server);
    }

    virtual ~ServerThread() {
        LOG_INFO("ServerThread end")
        server->setShouldRun(false);
        serverThread->join();
        delete serverThread;
    }

private:
    TcpServer *server = nullptr;
    std::thread *serverThread = nullptr;
};


#endif //TCP_SHMAFKA_SERVERPOSTTHREAD_H
