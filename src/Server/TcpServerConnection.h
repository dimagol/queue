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
#include "../Queue/ConcurentQueue.h"
#include "TcpServerOutcomeMessage.h"


using namespace boost::asio::ip;
using namespace std;


class ServerHandler;
class TcpServerConnection :
        public boost::enable_shared_from_this<TcpServerConnection>
{

public:
    typedef boost::shared_ptr<TcpServerConnection> TcpServerConnectionPointer;
    static TcpServerConnectionPointer create(boost::asio::io_service& io_service, ServerHandler *serverHandler)
    {
        return TcpServerConnectionPointer(new TcpServerConnection(io_service,serverHandler ));
    }

    tcp::socket& socket();
    void sendBulk(SocketProtoBuffer *buffer);
    void set_no_delay();
    void send_server_welcome();
    void send_server_goodbye();
private:
    explicit TcpServerConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler);
    void handle_send_welcome_message(const boost::system::error_code &errorCode, size_t);
    void handle_read_len(const boost::system::error_code &errorCode, size_t size);
    void handle_read_data(const boost::system::error_code &errorCode, size_t size);
    void handle_send_data(SocketProtoBuffer *buffer, const boost::system::error_code &errorCode, size_t size);
    void handle_send_server_goodbye(const boost::system::error_code &errorCode, size_t size);
    void close();

    static uint32_t client_id;
    ServerHandler * serverHandler;
    tcp::socket socket_;
    uint32_t len_in;
    SocketProtoBuffer * in;


    uint32_t id;
};


#endif //TCP_SHMAFKA_TCPCONNECTION_H
