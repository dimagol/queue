//
// Created by dima on 13/10/17.
//

#ifndef TCP_SHMAFKA_SERVER_H
#define TCP_SHMAFKA_SERVER_H
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
//#include <boost/enable_shared_from_this.hpp>
#include <utility>

#include "../DefinedMessages.h"
#include "TcpServerConnection.h"
#include "TcpServerIncomeMessage.h"
#include "TcpServerOutcomeMessage.h"
#include "../Msg/MsgBuilder.h"
#include "../Queue/ConcurrentQueueSingleConsumer.h"
#include "../Queue/ConcurrentQueue.h"


using namespace boost::asio::ip;
using namespace std;



class ServerHandler{
public:
    unordered_map<uint32_t , std::shared_ptr<TcpServerConnection>> client_map;


    ServerHandler(uint32_t queue_len, StrategyType waitingStrategy):
            concurentQueueFromClients(queue_len,waitingStrategy),
            concurentQueueToClient(queue_len,waitingStrategy),
            builder(nullptr)
    {}

    void register_client(std::shared_ptr<TcpServerConnection> conn, uint32_t id);
    void deregister_client(uint32_t id);

    ConcurrentQueueSingleConsumer<std::shared_ptr<TcpServerIncomeMessage>> concurentQueueFromClients;
    ConcurrentQueueSingleConsumer<std::shared_ptr<TcpServerOutcomeMessage>> concurentQueueToClient;
    MsgBuilder * builder;
};

class TcpServer
{
public:
    explicit TcpServer(uint16_t port, MsgBuilder * builder, uint32_t queueLen, StrategyType strategyType);

    void send(std::shared_ptr<TcpServerOutcomeMessage> &outMsg);

    std::shared_ptr<TcpServerIncomeMessage> tryRecieve();

    void run();

    void setShouldRun(volatile bool shouldRun);

    void disconnectClient(uint32_t id);

private:

    void start_accept();

    void handle_accept(std::shared_ptr<TcpServerConnection>  new_connection,
                       const boost::system::error_code& error);



private:
    boost::asio::io_service io_service;
    uint16_t port;
    tcp::acceptor acceptor_;
    volatile bool shouldRun = true;
    ServerHandler serverHandler;
    WaitingStrategy *waitingStrategy;



    void sendNormalMsg(std::shared_ptr<TcpServerOutcomeMessage> outMsg);

    void sendDisconnect(std::shared_ptr<TcpServerOutcomeMessage> outMsg);
};

#endif //TCP_SHMAFKA_SERVER_H
