#include "TcpServer.h"

void ServerHandler::register_client(TcpServerConnection::pointer conn, uint32_t id) {
    client_map[id] = std::move(conn);
}

void ServerHandler::deregister_client(uint32_t id) {
    if (client_map[id] != nullptr){
        client_map.erase(id);
    }
}

TcpServer::TcpServer(boost::asio::io_service &io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8081))
{
    start_accept();
}

void TcpServer::send(uint32_t id, SocketProtoBuffer *buffer) {
    serverHandler.concurentQueueToClient.push(make_shared<pair<uint32_t ,SocketProtoBuffer*>>(make_pair(id, buffer)));
}

shared_ptr<pair<uint32_t, SocketProtoBuffer *>> TcpServer::recieve() {
    return serverHandler.concurentQueueFromClients.pop();
}

void TcpServer::run() {
    while (shouldRun) {
        acceptor_.get_io_service().poll();
        shared_ptr<pair<uint32_t ,SocketProtoBuffer*>> msg = serverHandler.concurentQueueToClient.try_pop();
        if (msg != nullptr) {
            auto tcp_con = serverHandler.client_map[msg->first];
            ((TcpServerConnection::pointer) tcp_con)->send_data(msg->second);
        }
    }
}

void TcpServer::setShouldRun(volatile bool shouldRun) {
    TcpServer::shouldRun = shouldRun;
}

void TcpServer::start_accept() {
    TcpServerConnection::pointer new_connection =  TcpServerConnection::create(acceptor_.get_io_service(), &this->serverHandler);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&TcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void TcpServer::handle_accept(TcpServerConnection::pointer new_connection, const boost::system::error_code &error) {
    if (!error)
    {
        new_connection->set_no_delay();
        new_connection->send_server_welcome();
        start_accept();
    }
}
