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
#include "TcpServerIncomeMessage.h"


using namespace boost::asio::ip;
using namespace std;



class ServerHandler{
public:
    unordered_map<uint32_t , TcpServerConnection::TcpServerConnectionPointer> client_map;


    void register_client(TcpServerConnection::TcpServerConnectionPointer conn, uint32_t id);
    void deregister_client(uint32_t id);

    ConcurentQueue<TcpServerIncomeMessage> concurentQueueFromClients;
    ConcurentQueue<TcpServerIncomeMessage> concurentQueueToClient;
};

class TcpServer
{
public:
    explicit TcpServer(boost::asio::io_service& io_service, uint16_t port);


    TcpServerIncomeMessage recieve();

    void run();

    void setShouldRun(volatile bool shouldRun);

private:

    void start_accept();

    void handle_accept(TcpServerConnection::TcpServerConnectionPointer  new_connection,
                       const boost::system::error_code& error);



private:
    tcp::acceptor acceptor_;
    volatile bool shouldRun = true;
    ServerHandler serverHandler;

};

#endif //TCP_SHMAFKA_SERVER_H
