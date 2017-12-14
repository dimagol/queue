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

    void init(uint16_t port){
        LOG_INFO("ServerThread initiate port",port)
        this->port = port;
        server = new TcpServer(io_service_server,port);
    }

    void run(){
        serverThread = new thread(&TcpServer::run, server);
    }

public:
    virtual ~ServerThread() {
        LOG_INFO("ServerThread end port",port)
        server->setShouldRun(false);
        serverThread->join();
        delete serverThread;
        delete server;
    }

private:
    uint16_t port;
    boost::asio::io_service io_service_server;
    TcpServer *server;
    std::thread *serverThread;
};


#endif //TCP_SHMAFKA_SERVERPOSTTHREAD_H
