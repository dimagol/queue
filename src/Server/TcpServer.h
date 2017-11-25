//
// Created by dima on 13/10/17.
//

#ifndef TCP_SHMAFKA_SERVER_H
#define TCP_SHMAFKA_SERVER_H
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <boost/enable_shared_from_this.hpp>
#include <utility>

#include "../DefinedMessages.h"
#include "../ConcurentQueue.h"
#include "TcpServerConnection.h"


using namespace boost::asio::ip;
using namespace std;



class ServerHandler{
public:
    unordered_map<uint32_t , TcpServerConnection::pointer> client_map;


    void register_client(TcpServerConnection::pointer conn, uint32_t id);
    void deregister_client(uint32_t id);

    ConcurentQueue< shared_ptr<pair<uint32_t ,SocketProtoBuffer*>>> concurentQueueFromClients;
    ConcurentQueue< shared_ptr<pair<uint32_t ,SocketProtoBuffer*>>> concurentQueueToClient;
};

class TcpServer
{
public:
    explicit TcpServer(boost::asio::io_service& io_service);

    void send(uint32_t id,SocketProtoBuffer* buffer);

    shared_ptr<pair<uint32_t ,SocketProtoBuffer*>> recieve();

    void run();

    void setShouldRun(volatile bool shouldRun);

private:

    void start_accept();

    void handle_accept(TcpServerConnection::pointer  new_connection,
                       const boost::system::error_code& error);



private:
    tcp::acceptor acceptor_;
    volatile bool shouldRun = true;
    ServerHandler serverHandler;

};

#endif //TCP_SHMAFKA_SERVER_H
