//
// Created by dima on 24/11/17.
//

#ifndef TCP_SHMAFKA_CLIENT_H
#define TCP_SHMAFKA_CLIENT_H

//
// Client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "../Buff/BufferPool.h"
#include "../Queue/ConcurrentQueueSingleConsumer.h"
#include "../Logging/TSLogger.h"

using namespace std;
using boost::asio::ip::tcp;

class ClientBufferConteiner{
public:
    explicit ClientBufferConteiner(SocketProtoBuffer *first) :
            first(first),
            current(first),
            last(first){
        SocketProtoBuffer *next = first;
        while(next != nullptr){
            last = next;
            next = next->nextBuffer;
        }
    }

    SocketProtoBuffer* first;
    SocketProtoBuffer* last;
    SocketProtoBuffer* current;
};
class Client
{
public:
    Client(boost::asio::io_service& io_service, string &host, uint16_t port, StrategyType strategyType);
    std::thread spawn();
    void setShouldRun(volatile bool shouldRun);

    void run();

    void send(SocketProtoBuffer* buffer){
        concurentQueueToServer.push(buffer);
    }

    SocketProtoBuffer * recieve(){
        return concurentQueueFromServer.pop();
    }
    queue<SocketProtoBuffer *> queue1;
private:
    void handle_socket_connect(const boost::system::error_code &error);

    void handle_read_hello(const boost::system::error_code &error);

    void handle_read_len(const boost::system::error_code &error, size_t size);

    void handle_read_body(const boost::system::error_code &error, size_t size);

    void connectSocket();

    void registerForServerHello();

    void tryWrite();

    void tryRead();


    void handle_write(SocketProtoBuffer * out_buff, const boost::system::error_code &error, size_t size);

    void set_no_deley();

    void do_close() {
        tcpSocket.close();
    }

    void close() {
        io_service_.post(boost::bind(&Client::do_close, this));
    }





private:
    ConcurrentQueueSingleConsumer<SocketProtoBuffer *> concurentQueueToServer;
    ConcurrentQueueSingleConsumer<SocketProtoBuffer *> concurentQueueFromServer;

    string host;
    uint16_t port;

    SocketProtoBuffer * in_buff;

    volatile bool shouldRun = true;
    WaitingStrategy *waitingStrategy;
    tcp::resolver::iterator epIterator;
    boost::asio::io_service& io_service_;
    tcp::socket tcpSocket;

    bool socketConnected;
    bool appConnected;
    bool errorOccured;
    bool nothingToWrite;
    bool nothingToRead;

    void waitForSocketConnection() const;

    void waitForAppConnection();
};


#endif //TCP_SHMAFKA_CLIENT_H
