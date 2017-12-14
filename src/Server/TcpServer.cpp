#include "TcpServer.h"
#include "../Logging/TSLogger.h"

void ServerHandler::register_client(TcpServerConnection::TcpServerConnectionPointer conn, uint32_t id) {
    client_map[id] = std::move(conn);
}

void ServerHandler::deregister_client(uint32_t id) {
    if (client_map[id] != nullptr){
        client_map.erase(id);
    }
}

TcpServer::TcpServer(uint16_t port)
        : io_service(),
          port(port),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
    start_accept();
}


shared_ptr<TcpServerIncomeMessage> TcpServer::recieve() {
    return serverHandler.concurentQueueFromClients.try_pop();
}

void TcpServer::run() {
    LOG_INFO("tcp server start ")
    while (shouldRun) {
        acceptor_.get_io_service().poll();
        auto outMsg = serverHandler.concurentQueueToClient.try_pop();
        while (outMsg != nullptr){
            switch (outMsg->getType()){
                case TcpServerOutcomeMessage::DISCONNECT:
                    sendDisconnect(outMsg);
                case TcpServerOutcomeMessage::NORMAL:
                    sendNormalMsg(outMsg);
                    break;
                default:
                    LOG_ERROR("unsapported type ",outMsg->getType());
            }
            sendNormalMsg(outMsg);
        }

    }
}

void TcpServer::sendDisconnect(const shared_ptr<TcpServerOutcomeMessage> &outMsg) const {
    for (auto id : outMsg->getSendToSet()){
                        auto connPair = serverHandler.client_map.find(id);
                        if(__glibc_likely(connPair != serverHandler.client_map.end())){
                            connPair->second->send_server_goodbye();
                        } else{
                            LOG_WARN("cant send data to ",id);
                        }
                    }
}

void TcpServer::sendNormalMsg(const shared_ptr<TcpServerOutcomeMessage> &outMsg) const {
    uint32_t refCount = 0;
    for (auto id : outMsg->getSendToSet()){
                auto connPair = serverHandler.client_map.find(id);
                if(__glibc_likely(connPair != serverHandler.client_map.end())){
                    refCount++;
                    connPair->second->sendBulk(outMsg->getBuffer());
                } else{
                    LOG_WARN("cant send data to ",id);
                }
            }
    auto buff = outMsg->getBuffer();
    if(refCount == 0){
                BufferPool::bufferPool->releaseList(buff);
            } else{
                buff->setRefCountList(refCount);
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

void TcpServer::send(const shared_ptr<TcpServerOutcomeMessage> &outMsg) {
    serverHandler.concurentQueueToClient.push(outMsg);
}

void TcpServer::disconnectClient(uint32_t id){
    serverHandler.concurentQueueToClient.push(make_shared<TcpServerOutcomeMessage>(id, TcpServerOutcomeMessage::DISCONNECT));
}