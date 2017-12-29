//
// Created by dima on 24/11/17.
//

#include "TcpServerConnection.h"
#include "TcpServer.h"
#include "../Logging/TSLogger.h"

uint32_t TcpServerConnection::client_id = 0;

TcpServerConnection::TcpServerConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler)
        : socket_(new tcp::socket(io_service)),
          serverHandler(serverHandler){
    in = BufferPool::bufferPool->get();
    id = client_id++;
}


void TcpServerConnection::set_no_delay(){
    boost::asio::ip::tcp::no_delay option(true);
    socket_->set_option(option);
}


// send the welcome message
void TcpServerConnection::send_server_welcome() {
    boost::asio::async_write(*socket_,
                             boost::asio::buffer(DefinedMessages::hello_msg->msg_complete_buff,
                                                 DefinedMessages::hello_msg->get_msg_all_data_len()),
                             boost::bind(&TcpServerConnection::handle_send_welcome_message,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));


    serverHandler->register_client(shared_from_this(),id);
}

void TcpServerConnection::handle_send_welcome_message(const boost::system::error_code &errorCode, size_t size) {
    if (__glibc_unlikely(errorCode != nullptr)){
        LOG_WARN("error:" ,errorCode);
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(*socket_,
                            boost::asio::buffer(in->msg_len_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&TcpServerConnection::handle_read_len,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}



// send the welcome message
void TcpServerConnection::send_server_goodbye() {
    boost::asio::async_write(*socket_,
                             boost::asio::buffer(DefinedMessages::goodbye_msg->msg_complete_buff,
                                                 DefinedMessages::goodbye_msg->get_msg_all_data_len()),
                             boost::bind(&TcpServerConnection::handle_send_server_goodbye,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void TcpServerConnection::handle_send_server_goodbye(const boost::system::error_code &errorCode, size_t size) {
    serverHandler->deregister_client(id);
    BufferPool::bufferPool->release(in);
    if (__glibc_unlikely(errorCode != nullptr)){
        LOG_WARN("error:" ,errorCode);
        return;
    }
}



void TcpServerConnection::handle_read_len(const boost::system::error_code &errorCode, size_t size){
    if (__glibc_unlikely(errorCode != nullptr)){
        LOG_WARN("error:" ,errorCode);
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }

    if(__glibc_unlikely(size != 4)){
        LOG_WARN("size != 4 ", size) ;
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }

    len_in = in->get_msg_len();
    if(__glibc_unlikely(len_in > in->len || (errorCode != nullptr))){
        LOG_WARN("got to big msg or error");
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }
    boost::asio::async_read(*socket_,
                            boost::asio::buffer(in->msg_data_buff, len_in),
                            boost::bind(&TcpServerConnection::handle_read_data,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpServerConnection::handle_read_data(const boost::system::error_code &errorCode, size_t size) {

    if (__glibc_unlikely(errorCode != nullptr)){
        LOG_WARN("error:" ,errorCode);
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }

    if (__glibc_unlikely(len_in != size)){
        LOG_WARN("got bad size : " ,len_in ," ", size);
        BufferPool::bufferPool->release(in);
        serverHandler->deregister_client(id);
        return;
    }
    serverHandler->concurentQueueFromClients.push(make_shared<TcpServerIncomeMessage>(in,id));

    in = BufferPool::bufferPool->get();
    boost::asio::async_read(*socket_,
                            boost::asio::buffer(in->msg_complete_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&TcpServerConnection::handle_read_len,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TcpServerConnection::sendBulk(SocketProtoBuffer *buffer) {
    boost::asio::async_write(*socket_,
                             boost::asio::buffer(buffer->msg_complete_buff, buffer->get_msg_all_data_len()),
                             boost::bind(&TcpServerConnection::handle_send_data,
                                         shared_from_this(),
                                         buffer,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

tcp::socket * TcpServerConnection::socket() {
    return socket_;
}

void TcpServerConnection::handle_send_data(SocketProtoBuffer *buffer, const boost::system::error_code &errorCode, size_t size) {
    if (__glibc_unlikely(errorCode != nullptr)){
        LOG_WARN("error:" ,errorCode);
        auto tmpBuffer = buffer->nextBuffer;
        while (tmpBuffer != nullptr){
            tmpBuffer->decRef();
        }
        serverHandler->deregister_client(id);
        return;
    }
    if(buffer->nextBuffer != nullptr){
        sendBulk(buffer->nextBuffer);
    }
    buffer->decRef();
    if(buffer->sendingRefCount == 0){
        BufferPool::bufferPool->releaseOne(buffer);
    }

}

void TcpServerConnection::close() {
    if(socket_ != nullptr){
        socket_->close();
        delete socket_;
        socket_ = nullptr;
    }
}

