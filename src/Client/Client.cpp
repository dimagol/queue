//
// Created by dima on 24/11/17.
//

#include <cstdint>

#include "Client.h"
#include "../DefinedMessages.h"
#include "../Logging/TSLogger.h"

Client::Client(boost::asio::io_service &io_service, string &host, uint16_t port, StrategyType strategyType)
        : io_service_(io_service),
          tcpSocket(io_service),
          port(port),
          host(host),
          concurentQueueFromServer(1024,SLEEP),
          concurentQueueToServer(1024,SLEEP),
          waitingStrategy(getStrategy(strategyType)),
          socketConnected(false),
          appConnected(false),
          errorOccured(false)
{
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host,to_string((int)port));
    epIterator = resolver.resolve(query);
    in_buff = BufferPool::bufferPool->get();
    LOG_INFO("client created ", host, ":", port)
}

void Client::connectSocket() {
    LOG_INFO("connecting to ", host, ":", port);
    boost::asio::async_connect(tcpSocket, epIterator,
                               boost::bind(&Client::handle_socket_connect, this,
                                           boost::asio::placeholders::error));

}

void Client::handle_socket_connect(const boost::system::error_code &error) {
    if (error == nullptr){
        set_no_deley();
        socketConnected = true;
        LOG_INFO("connected to ", host, ":", port);
//        registerForServerHello();
    } else{
        socketConnected = false;
        errorOccured = true;
        LOG_ERROR("unable to connect to ", host, ":", port);
    }
}

//validate connect msg and register_and_send_server_welcome negotiation
void Client::handle_read_hello(const boost::system::error_code &error) {
    LOG_ERROR("zzzzzzzzzzzzzzzzz");
    if (error != nullptr)
    {
        errorOccured = true;
        LOG_ERROR("got error " , error );
        return;
    }
//    if(size != DefinedMessages::hello_msg->get_msg_all_data_len()){
//        errorOccured = true;
//        LOG_ERROR("got unexpected msg len, expected: ",
//                  DefinedMessages::hello_msg->get_msg_all_data_len(),
//                  " got: ",
//                  size)
//        return;
//    }
    if( in_buff->get_msg_len() != DefinedMessages::hello_msg->get_msg_len()){
        errorOccured = true;
        LOG_ERROR("bad welcome message len, expected: " ,
                  DefinedMessages::hello_msg->get_msg_len() ,
                  " got: ",
                  in_buff->get_msg_len());
        return;
    }
    if (strcmp((const char *)(in_buff->msg_data_buff),
               (const char *)(DefinedMessages::hello_msg->msg_data_buff)) != 0){
        LOG_ERROR("bad welcome message");
        errorOccured = true;
        return;
    }
    appConnected = true;

}

void Client::registerForServerHello() {
    // handle read hello
//    LOG_ERROR(DefinedMessages::hello_msg->get_msg_all_data_len())
    LOG_ERROR("aaaaaaaaaaaa");
    boost::asio::async_read(tcpSocket,
                            boost::asio::buffer(in_buff->msg_complete_buff, DefinedMessages::hello_msg->get_msg_all_data_len()),
                            boost::bind(&Client::handle_read_hello,
                                        this,
                                        boost::asio::placeholders::error));
//    tcpSocket.get_io_service().run();
}



void Client::tryRead() {
    if (tcpSocket.available() >= 0){
        nothingToRead = false;
        boost::asio::async_read(tcpSocket,
                                boost::asio::buffer(in_buff->msg_len_buff, MSG_LEN_BUFF_LEN),
                                boost::bind(&Client::handle_read_len,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    } else{
        nothingToRead = true;
    }

}

void Client::handle_read_len(const boost::system::error_code &error, size_t size) {
    if (error != nullptr) {
        LOG_ERROR("got error " , error );
        errorOccured = true;
        return;
    }

    if(size != MSG_LEN_BUFF_LEN){
        LOG_ERROR("got unexpected read len, expected: ", MSG_LEN_BUFF_LEN, "got: ", size);
        errorOccured = true;
        return;
    }

    uint32_t lenIn  = in_buff->get_msg_len();
    if(lenIn > in_buff->len){
        LOG_ERROR("bad len ", lenIn);
        errorOccured = true;
        return;
    }

    boost::asio::async_read(tcpSocket,
                            boost::asio::buffer(in_buff->msg_data_buff, lenIn),
                            boost::bind(&Client::handle_read_body,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void Client::handle_read_body(const boost::system::error_code &error, size_t size) {
    if (error != nullptr) {
        errorOccured = true;
        LOG_ERROR("got error " , error );
        return;
    }

    if(size != in_buff->get_msg_len()){
        errorOccured = true;
        LOG_ERROR("got unexpected read len, expected: ", in_buff->get_msg_len(), "got: ", size);
        return;
    }

    concurentQueueFromServer.push(in_buff);
    in_buff =  BufferPool::bufferPool->get();
    if(in_buff == nullptr){
        errorOccured = true;
        LOG_ERROR("unable to get buffer")
        return;
    }
    tryRead();

}

void Client::tryWrite() {
        auto buff = concurentQueueToServer.try_pop();
        if (buff != nullptr) {
            boost::asio::async_write(tcpSocket,
                                     boost::asio::buffer(buff->msg_complete_buff,
                                                         buff->get_msg_all_data_len()),
                                     boost::bind(&Client::handle_write,
                                                 this,
                                                 buff,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
            nothingToWrite = false;
        } else {
            nothingToWrite = true;
        }
}

void Client::handle_write(SocketProtoBuffer *  out_buff,
                          const boost::system::error_code &error,
                          size_t size) {
    if(error != nullptr){
        LOG_ERROR("got error " , error );
        errorOccured = true;
        return;
    }

    if(size != out_buff->get_msg_all_data_len()){
        LOG_ERROR("got unexpected write len, expected: ", out_buff->get_msg_len(), "got: ", size);
        errorOccured = true;
        return;
    }

    if(out_buff->nextBuffer != nullptr){
        boost::asio::async_write(tcpSocket,
                                 boost::asio::buffer(out_buff->nextBuffer->msg_complete_buff,
                                                     out_buff->nextBuffer->get_msg_all_data_len()),
                                 boost::bind(&Client::handle_write,
                                             this,
                                             out_buff->nextBuffer,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        nothingToWrite = false;
        BufferPool::bufferPool->releaseOne(out_buff);
        return;
    }
    tryWrite();
}

void Client::setShouldRun(volatile bool shouldRun) {
    Client::shouldRun = shouldRun;
}

void Client::set_no_deley() {
    boost::asio::ip::tcp::no_delay option(true);
    tcpSocket.set_option(option);
}

std::thread Client::spawn() {
    return std::thread(&Client::run, this);
}

void Client::run() {
    connectSocket();
    waitForSocketConnection();
    if(errorOccured || !socketConnected){
        LOG_ERROR("error during connection")
        return;
    }
    registerForServerHello();
//    while (true){
//        if(tcpSocket.available()){
//            LOG_INFO(tcpSocket.available())
//        }
//    }

    waitForAppConnection();

    if(errorOccured || !appConnected){
        LOG_ERROR("error during hello")
        return;
    }

    tryRead();
    tryWrite();

    while (shouldRun){
        io_service_.poll();
        if(errorOccured){
            LOG_ERROR("error during running")
            return;
        }
        if(nothingToRead && nothingToWrite){
            waitingStrategy->wait();
        }
        if(nothingToWrite){
            tryWrite();
        }
        if(nothingToRead){
            tryRead();
        }
    }

    close();
}

void Client::waitForAppConnection() {
    while (!errorOccured && !appConnected){
        size_t handelersCalled  = tcpSocket.get_io_service().poll_one();
//        size_t handelersCalled = s;
//        io_service_.`
//        LOG_ERROR(tcpSocket.available());
        if(handelersCalled == 0){
            waitingStrategy->wait();
        } else {
            LOG_ERROR("ssssss");
        }
    }
}

void Client::waitForSocketConnection() const {
    while (!socketConnected && !errorOccured){
        size_t handelersCalled = io_service_.poll_one();
        if(handelersCalled == 0){
            waitingStrategy->wait();
        }
    }
}






