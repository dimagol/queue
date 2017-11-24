//
// Created by dima on 24/11/17.
//

#ifndef TCP_SHMAFKA_TCPCONNECTION_H
#define TCP_SHMAFKA_TCPCONNECTION_H

#include <boost/asio.hpp>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <cstdint>
#include <boost/enable_shared_from_this.hpp>

#include "../DefinedMessages.h"
#include "../ConcurentQueue.h"


using namespace boost::asio::ip;
using namespace std;


class ServerHandler;
class TcpConnection :
        public boost::enable_shared_from_this<TcpConnection>
{

public:
    typedef boost::shared_ptr<TcpConnection> pointer;
    static pointer create(boost::asio::io_service& io_service, ServerHandler *serverHandler)
    {
        return pointer(new TcpConnection(io_service,serverHandler ));
    }
    static uint32_t client_id;
    tcp::socket& socket();
    void start();
    void set_no_deley();

    void send_data(SocketProtoBuffer * buffer);
private:
    explicit TcpConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler);
    void handle_write_first(const boost::system::error_code & /*error*/,
                            size_t /*bytes_transferred*/);
    void handle_read_first(const boost::system::error_code & /*error*/,
                           size_t /*bytes_transferred*/);
    void handle_read_all(const boost::system::error_code & /*error*/,
                         size_t /*bytes_transferred*/);


    ServerHandler * serverHandler;
    tcp::socket socket_;
    uint32_t len_in;
    SocketProtoBuffer * in;
    uint32_t id;
};


#endif //TCP_SHMAFKA_TCPCONNECTION_H
