//

#include "server.h"
// Created by dima on 13/10/17.
//
//uint32_t TcpConnection::client_id = 0;
//
//TcpConnection::TcpConnection(boost::asio::io_service& io_service, ServerHandler *serverHandler)
//        : socket_(io_service),serverHandler(serverHandler){
//    in = BufferPool::bufferPool->get();
//    id = client_id++;
//}
//
//
//void TcpConnection::set_no_deley(){
//    boost::asio::ip::tcp::no_delay option(true);
//    socket_.set_option(option);
//}
//
//
//// send the welcome message
//void TcpConnection::start() {
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(DefinedMessages::hello_msg->buff, DefinedMessages::hello_msg->len),
//                             boost::bind(&TcpConnection::handle_write_first,
//                                         shared_from_this(),
//                                         boost::asio::placeholders::error,
//                                         boost::asio::placeholders::bytes_transferred));
//
//
//    serverHandler->register_client(shared_from_this(),id);
//}
//
//void TcpConnection::handle_write_first(const boost::system::error_code &errorCode,
//                                        size_t size) {
//    if (errorCode){
//        cerr << "got to big msg or error" << endl;
//        serverHandler->deregister_client(id);
//        return;
//    }
//    boost::asio::async_read(socket_,
//                            boost::asio::buffer(in->buff, 4),
//                            boost::bind(&TcpConnection::handle_read_first,
//                                        shared_from_this(),
//                                        boost::asio::placeholders::error,
//                                        boost::asio::placeholders::bytes_transferred));
//}
//void TcpConnection::handle_read_first(const boost::system::error_code &errorCode /*error*/,
//                                       size_t size /*bytes_transferred*/){
//    cout << errorCode << endl;
//
//    len_in = ntohl(*((uint32_t *)in->buff));
//    if(len_in > in->len || errorCode){
//        cerr << "got to big msg or error" << endl;
//        serverHandler->deregister_client(id);
//        return;
//    }
//    boost::asio::async_read(socket_,
//                            boost::asio::buffer(in->buff, len_in),
//                            boost::bind(&TcpConnection::handle_read_all,shared_from_this() ,
//                                        boost::asio::placeholders::error,
//                                        boost::asio::placeholders::bytes_transferred));
//}
//
//void TcpConnection::handle_read_all(const boost::system::error_code & errorCode/*error*/,
//                     size_t /*bytes_transferred*/) {
//
//    if (errorCode){
//        cerr << "got to big msg or error" << endl;
//        serverHandler->deregister_client(id);
//        return;
//    }
//
//    in = BufferPool::bufferPool->get();
//    boost::asio::async_read(socket_,
//                            boost::asio::buffer(in->buff, len_in),
//                            boost::bind(&TcpConnection::handle_read_first, this,
//                                        boost::asio::placeholders::error,
//                                        boost::asio::placeholders::bytes_transferred));
//}
//
//void TcpConnection::send_data(Buffer *buffer) {
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(buffer->buff, buffer->len),
//                             boost::bind(&TcpConnection::handle_write_first,
//                                         this,
//                                         boost::asio::placeholders::error,
//                                         boost::asio::placeholders::bytes_transferred));
//}
//
//tcp::socket& TcpConnection::socket() {
//    return socket_;
//}
