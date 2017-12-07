#include "TcpServer.h"

void ServerHandler::register_client(TcpServerConnection::TcpServerConnectionPointer conn, uint32_t id) {
    client_map[id] = std::move(conn);
}

void ServerHandler::deregister_client(uint32_t id) {
    if (client_map[id] != nullptr){
        client_map.erase(id);
    }
}

TcpServer::TcpServer(boost::asio::io_service &io_service, uint16_t port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
    start_accept();
}


TcpServerIncomeMessage TcpServer::recieve() {
    return serverHandler.concurentQueueFromClients.pop();
}

void TcpServer::run() {
    while (shouldRun) {
        acceptor_.get_io_service().poll();
    }
}

void TcpServer::setShouldRun(volatile bool shouldRun) {
    TcpServer::shouldRun = shouldRun;
}

void TcpServer::start_accept() {
    TcpServerConnection::TcpServerConnectionPointer new_connection =  TcpServerConnection::create(acceptor_.get_io_service(), &this->serverHandler);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&TcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void TcpServer::handle_accept(TcpServerConnection::TcpServerConnectionPointer new_connection, const boost::system::error_code &error) {
    if (!error)
    {
        new_connection->set_no_delay();
        new_connection->send_server_welcome();
        start_accept();
    }
}
