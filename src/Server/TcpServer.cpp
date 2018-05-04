#include "TcpServer.h"
#include "../Logging/TSLogger.h"

void ServerHandler::register_client(std::shared_ptr<TcpServerConnection> conn, uint32_t id) {
    client_map[id] = conn;
}

void ServerHandler::deregister_client(uint32_t id) {
    auto fromMap = client_map.find(id);
    if (fromMap != client_map.end()){
        fromMap->second->close();
        client_map.erase(id);
        LOG_WARN("removed " ,id)
        auto z = make_shared<TcpServerIncomeMessage>(builder->buildDisconnectMsg(),id);
        concurentQueueFromClients.push(z);
    } else{
        LOG_ERROR("already removed " ,id)
    }
}

TcpServer::TcpServer(uint16_t port,
                     MsgBuilder * builder,
                     uint32_t queueLen,
                     StrategyType strategyType):
          io_service(),
          port(port),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
          serverHandler(queueLen,strategyType),
          waitingStrategy(getStrategy(strategyType))

{
    serverHandler.builder = builder;
    start_accept();
}


shared_ptr<TcpServerIncomeMessage> TcpServer::tryRecieve() {
    return serverHandler.concurentQueueFromClients.try_pop();
}



void TcpServer::sendDisconnect( shared_ptr<TcpServerOutcomeMessage> outMsg)  {
    for (auto id : outMsg->getSendToSet()){
        uint32_t idd = id;
        auto conn = serverHandler.client_map.find(idd);
        if(__glibc_likely(conn != serverHandler.client_map.end())){
            LOG_ERROR(id)
            conn->second->send_server_goodbye();
        } else{
            LOG_WARN("cant send data to ",id);
        }
    }
}

void TcpServer::sendNormalMsg( shared_ptr<TcpServerOutcomeMessage> outMsg)  {
    uint32_t refCount = 0;
    for (auto id : outMsg->getSendToSet()){
                auto connPair = serverHandler.client_map.find(id);
                if(__glibc_likely(connPair != serverHandler.client_map.end())){
                    refCount++;
                    if(!connPair->second->sendBulk(outMsg->getBuffer())){
                        LOG_ERROR("client queue is full id:" ,id);
                        refCount--;
                    }
                } else{
                    LOG_WARN("cant send data to ",id);
                }
    }
    auto buff = outMsg->getBuffer();
    if(refCount == 0){
        BufferPool::bufferPool->release(buff);
    } else{
        buff->setRefCountList(refCount);
    }
}

void TcpServer::setShouldRun(volatile bool shouldRun) {
    TcpServer::shouldRun = shouldRun;
}

void TcpServer::start_accept() {
    std::shared_ptr<TcpServerConnection> new_connection =  TcpServerConnection::create(acceptor_.get_io_service(), &this->serverHandler);
    acceptor_.async_accept(*(new_connection->socket()),
                           boost::bind(&TcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void TcpServer::handle_accept(std::shared_ptr<TcpServerConnection> new_connection, const boost::system::error_code &error) {
    if (!error)
    {
        LOG_INFO("got new connection")
        new_connection->set_no_delay();
        new_connection->register_and_send_server_welcome();
        start_accept();

    } else {
        LOG_ERROR("unable to accept")
    }
}

void TcpServer::send(shared_ptr<TcpServerOutcomeMessage> &outMsg) {
    serverHandler.concurentQueueToClient.push(outMsg);
}

void TcpServer::run() {
    LOG_INFO("tcp server start ")
    uint32_t pps=0;
    time_t seconds = time (NULL);
    while (shouldRun) {

        for(auto&& client : serverHandler.client_map){
            client.second->tryWrite();
            client.second->tryRead();
        }
        size_t numOfhandlersExecuted = acceptor_.get_io_service().poll();


        auto outMsg = serverHandler.concurentQueueToClient.try_pop();
        if (outMsg != nullptr){
            pps++;
            if (seconds != time (NULL)){
                seconds = time (NULL);
                cout << "pps : " << pps <<endl;
                pps = 0;
            }
            switch (outMsg->getType()){
                case TcpServerOutcomeMessage::DISCONNECT:
                    sendDisconnect(outMsg);
                    break;
                case TcpServerOutcomeMessage::NORMAL:
                    sendNormalMsg(outMsg);
                    break;
                default:
                    LOG_ERROR("unsapported type ",outMsg->getType());
            }
        }
        if (numOfhandlersExecuted == 0 && outMsg == nullptr){
            waitingStrategy->wait();
        }

    }
}

void TcpServer::disconnectClient(uint32_t id){
    auto z = make_shared<TcpServerOutcomeMessage>(id, TcpServerOutcomeMessage::DISCONNECT);
    serverHandler.concurentQueueToClient.push(z);
}