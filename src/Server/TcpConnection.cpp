//
// Created by dima on 24/11/17.
//

#include "TcpConnection.h"

#include "server.h"
// Created by dima on 13/10/17.

uint32_t TcpConnection::client_id = 0;

TcpConnection::TcpConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler)
        : socket_(io_service),serverHandler(serverHandler){
    in = BufferPool::bufferPool->get();
    id = client_id++;
}


void TcpConnection::set_no_deley(){
    boost::asio::ip::tcp::no_delay option(true);
    socket_.set_option(option);
}


// send the welcome message
void TcpConnection::start() {
    cout << "TcpConnection::start" << endl;
    boost::asio::async_write(socket_,
                             boost::asio::buffer(DefinedMessages::hello_msg->msg_data_buff, DefinedMessages::hello_msg->len),
                             boost::bind(&TcpConnection::handle_write_first,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));


    serverHandler->register_client(shared_from_this(),id);
}

void TcpConnection::handle_write_first(const boost::system::error_code &errorCode,
                                       size_t size) {
    cout << "TcpConnection::handle_write_first" << endl;
    if (errorCode != nullptr){
        cerr << "error" << errorCode << endl;
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_data_buff, 4),
                            boost::bind(&TcpConnection::handle_read_first,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}
void TcpConnection::handle_read_first(const boost::system::error_code &errorCode /*error*/,
                                      size_t size /*bytes_transferred*/){
    cout << "TcpConnection::handle_read_first" << endl;
    cout << errorCode << endl;

    len_in = in->get_msg_len();
    if(len_in > in->len || (errorCode != nullptr)){
        cerr << "got to big msg or error" << endl;
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_data_buff + 4, len_in),
                            boost::bind(&TcpConnection::handle_read_all,shared_from_this() ,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpConnection::handle_read_all(const boost::system::error_code & errorCode/*error*/,
                                    size_t /*bytes_transferred*/) {

    cout << "TcpConnection::handle_read_all" << endl;
    if (errorCode){
        cerr << "got to big msg or error" << endl;
        serverHandler->deregister_client(id);
        return;
    }

    in = BufferPool::bufferPool->get();
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_data_buff, len_in),
                            boost::bind(&TcpConnection::handle_read_first, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpConnection::send_data(SocketProtoBuffer *buffer) {
    cout << "TcpConnection::send_data" << endl;
    boost::asio::async_write(socket_,
                             boost::asio::buffer(buffer->msg_data_buff, buffer->len),
                             boost::bind(&TcpConnection::handle_write_first,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

tcp::socket& TcpConnection::socket() {
    return socket_;
}
