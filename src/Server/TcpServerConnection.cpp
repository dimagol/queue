//
// Created by dima on 24/11/17.
//

#include "TcpServerConnection.h"
#include "TcpServer.h"

uint32_t TcpServerConnection::client_id = 0;

TcpServerConnection::TcpServerConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler)
        : socket_(io_service),
          serverHandler(serverHandler){
    in = BufferPool::bufferPool->get();
    id = client_id++;
}


void TcpServerConnection::set_no_delay(){
    boost::asio::ip::tcp::no_delay option(true);
    socket_.set_option(option);
}


// send the welcome message
void TcpServerConnection::send_server_welcome() {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(DefinedMessages::hello_msg->all_data,
                                                 DefinedMessages::hello_msg->get_msg_all_data_len()),
                             boost::bind(&TcpServerConnection::handle_send_welcome_message,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));


    serverHandler->register_client(shared_from_this(),id);
}

void TcpServerConnection::handle_send_welcome_message(const boost::system::error_code &errorCode, size_t size) {
    if (errorCode != nullptr){
        cerr << "error" << errorCode << endl;
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_len_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&TcpServerConnection::handle_read_len,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}
void TcpServerConnection::handle_read_len(const boost::system::error_code &errorCode, size_t size){

    if(size != 4){
        cerr << "size != 4 \n" ;
    }

    len_in = in->get_msg_len();
    if(len_in > in->len || (errorCode != nullptr)){
        cerr << "got to big msg or error" << endl;
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_data_buff, len_in),
                            boost::bind(&TcpServerConnection::handle_read_data,shared_from_this() ,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpServerConnection::handle_read_data(const boost::system::error_code &errorCode, size_t size) {

    if (errorCode != nullptr){
        cerr << "got to big msg or error" << endl;
        serverHandler->deregister_client(id);
        return;
    }
    serverHandler->concurentQueueFromClients.push(TcpServerIncomeMessage(in,id));
    in = BufferPool::bufferPool->get();
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in->msg_data_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&TcpServerConnection::handle_read_len,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpServerConnection::send_data(SocketProtoBuffer *buffer) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(buffer->all_data, buffer->get_msg_all_data_len()),
                             boost::bind(&TcpServerConnection::handle_send_data,
                                         this,
                                         buffer,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

tcp::socket& TcpServerConnection::socket() {
    return socket_;
}

void TcpServerConnection::handle_send_data(SocketProtoBuffer *buffer, const boost::system::error_code &errorCode, size_t size) {
    BufferPool::bufferPool->release(buffer);
    if (errorCode != nullptr){
        cerr << "handle_send_data error" << endl;
        serverHandler->deregister_client(id);
        return;
    }
}
