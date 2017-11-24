//
// Created by dima on 13/10/17.
//

#ifndef TCP_SHMAFKA_SERVER_H
#define TCP_SHMAFKA_SERVER_H
#include <boost/asio.hpp>
#include <ctime>
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

//
//class ServerHandler;
//class TcpConnection :
//        public boost::enable_shared_from_this<TcpConnection>
//{
//
//public:
//    typedef boost::shared_ptr<TcpConnection> pointer;
//    static pointer create(boost::asio::io_service& io_service, ServerHandler *serverHandler)
//    {
//        return pointer(new TcpConnection(io_service,serverHandler ));
//    }
//    static uint32_t client_id;
//    tcp::socket& socket();
//    void start();
//    void set_no_deley();
//
//    void send_data(Buffer * buffer);
//private:
//    explicit TcpConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler);
//    void handle_write_first(const boost::system::error_code & /*error*/,
//                            size_t /*bytes_transferred*/);
//    void handle_read_first(const boost::system::error_code & /*error*/,
//                           size_t /*bytes_transferred*/);
//    void handle_read_all(const boost::system::error_code & /*error*/,
//                           size_t /*bytes_transferred*/);
//
//
//    ServerHandler * serverHandler;
//    uint32_t len_in;
//    tcp::socket socket_;
//    Buffer * in;
//    uint32_t id;
//};


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

    ConcurentQueue< pair<uint32_t ,Buffer*> *> concurentQueueIn;
    ConcurentQueue< pair<uint32_t ,Buffer*> *> concurentQueueOut;
};

class tcp_server
{
public:
    explicit tcp_server(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8081))
    {
        start_accept();
    }

    void run(){
        acceptor_.get_io_service().poll();
        auto msg = serverHandler.concurentQueueOut.try_pop();
        while (msg!= nullptr) {
            auto tcp_con = serverHandler.client_map[msg->first];
            if (tcp_con != nullptr) {
                ((TcpConnection::pointer) tcp_con)->send_data(msg->second);
            }
            msg = serverHandler.concurentQueueOut.try_pop();
        }
    }

public:
    ServerHandler serverHandler;
    void start_accept()
    {
        TcpConnection::pointer new_connection =  TcpConnection::create(acceptor_.get_io_service(), &this->serverHandler);

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
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

    tcp::acceptor acceptor_;

public:

};

#endif //TCP_SHMAFKA_SERVER_H
