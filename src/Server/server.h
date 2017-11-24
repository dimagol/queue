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
#include "TcpConnection.h"


using namespace boost::asio::ip;
using namespace std;



class ServerHandler{
public:
    unordered_map<uint32_t , TcpConnection::pointer> client_map;


    void register_client(TcpConnection::pointer conn, uint32_t id){
        client_map[id] = std::move(conn);
    }
    void deregister_client(uint32_t id){
        if (client_map[id] != nullptr){
            client_map.erase(id);
        }
    }

    ConcurentQueue< pair<uint32_t ,SocketProtoBuffer*> *> concurentQueueIn;
    ConcurentQueue< pair<uint32_t ,SocketProtoBuffer*> *> concurentQueueToClient;
};

class TcpServer
{
public:
    explicit TcpServer(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8081))
    {
        start_accept();
    }

    void run(){
        while (shouldRun) {
            acceptor_.get_io_service().poll();
            auto msg = serverHandler.concurentQueueToClient.try_pop();
            if (msg!= nullptr) {
                auto tcp_con = serverHandler.client_map[msg->first];
                ((TcpConnection::pointer) tcp_con)->send_data(msg->second);
            }
        }
    }

    void setShouldRun(volatile bool shouldRun) {
        TcpServer::shouldRun = shouldRun;
    }

private:

    void start_accept()
    {
        TcpConnection::pointer new_connection =  TcpConnection::create(acceptor_.get_io_service(), &this->serverHandler);

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&TcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void handle_accept(TcpConnection::pointer  new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->set_no_deley();
            new_connection->start();
            start_accept();
        }
    }



private:
    tcp::acceptor acceptor_;
    volatile bool shouldRun = true;
    ServerHandler serverHandler;

};

#endif //TCP_SHMAFKA_SERVER_H
