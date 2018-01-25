//
// Created by dima on 24/11/17.
//

#include <cstdint>

#include "Client.h"
#include "../DefinedMessages.h"
#include "../Logging/TSLogger.h"

Client::Client(boost::asio::io_service &io_service, string &host, uint16_t port)
        : io_service_(io_service),
          socket_(io_service),
          port(port),
          host(host)
{
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host,to_string((int)port));
    epIterator = resolver.resolve(query);
    in_buff = BufferPool::bufferPool->get();

    // wait for server hello
    boost::asio::async_connect(socket_, epIterator,
                               boost::bind(&Client::handle_connect, this,
                                           boost::asio::placeholders::error));
}
//getChunkedWithIntAndStr connect msg
void Client::handle_connect(const boost::system::error_code &error) {
    if (!error)
    {
        set_no_deley();
        boost::asio::async_read(socket_,
                                boost::asio::buffer(in_buff->msg_complete_buff, DefinedMessages::hello_msg->get_msg_all_data_len()),
                                boost::bind(&Client::handle_read_hello, this,
                                            boost::asio::placeholders::error));
    } else{
        LOG_ERROR("unable to connect");
    }
}

//validate connect msg and send_server_welcome negotiation
void Client::handle_read_hello(const boost::system::error_code &error) {
//    cout << "Client::handle_read_hello" << endl;
    if (error != nullptr)
    {
        cerr << "got error" << error << endl;
        do_close();
        return;
    }
    if( in_buff->get_msg_len() != DefinedMessages::hello_msg->get_msg_len()){
        cerr << "bad welcome message len" <<DefinedMessages::hello_msg->get_msg_len() << " " << in_buff->get_msg_len() << endl;
        return;
    }

    if (strcmp((const char *)(in_buff->msg_data_buff),
               (const char *)(DefinedMessages::hello_msg->msg_data_buff)) != 0){
        cerr << "bad welcome message" << endl;
        return;
    }


    boost::asio::async_read(socket_,
                            boost::asio::buffer(in_buff->msg_len_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&Client::handle_read_len,
                                        this,
                                        boost::asio::placeholders::error));
}

void Client::handle_read_len(const boost::system::error_code &error) {
    uint32_t len  = in_buff->get_msg_len();
    if(len > in_buff->len){
        cerr << "bad len " <<  in_buff->get_msg_len() << endl;
        do_close();
        return;
    }

    if (error != nullptr) {
        cerr << "got error " << error << endl;
        do_close();
        return;
    }

    boost::asio::async_read(socket_,
                            boost::asio::buffer(in_buff->msg_data_buff, len),
                            boost::bind(&Client::handle_body, this,
                                        boost::asio::placeholders::error));
}

void Client::handle_body(const boost::system::error_code &error) {
    if (error != nullptr) {
        cerr << "got error " << error << endl;
        do_close();
        return;
    }
    concurentQueueFromServer.push(in_buff);
    in_buff =  BufferPool::bufferPool->get();
    boost::asio::async_read(socket_,
                            boost::asio::buffer(in_buff->msg_len_buff, MSG_LEN_BUFF_LEN),
                            boost::bind(&Client::handle_read_len,
                                        this,
                                        boost::asio::placeholders::error));
}

void Client::write(shared_ptr<ClientBufferConteiner> buffer) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(buffer->current->msg_complete_buff,
                                                 buffer->current->get_msg_all_data_len()),
                             boost::bind(&Client::handle_write,
                                         this,
                                         buffer,
                                         boost::asio::placeholders::error));
}

void Client::handle_write(shared_ptr<ClientBufferConteiner> out_buff, const boost::system::error_code &error) {
    if(error != nullptr){
        cerr << "got error " << error << endl;
        BufferPool::bufferPool->release(out_buff->first);
    }
    if (out_buff->current != out_buff->last){
        out_buff->current = out_buff->current->nextBuffer;
        write(out_buff);
    } else{
        BufferPool::bufferPool->release(out_buff->first);
    }
}

void Client::setShouldRun(volatile bool shouldRun) {
    Client::shouldRun = shouldRun;
}

void Client::set_no_deley() {
    boost::asio::ip::tcp::no_delay option(true);
    socket_.set_option(option);
}

std::thread Client::spawn() {
    return std::thread(&Client::run, this);
}
