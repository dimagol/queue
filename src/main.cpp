#include <iostream>
#include "Server/TcpServer.h"
#include "Client/Client.h"
#include "Logging/TSLogger.h"
#include "Chain/Chain.h"

int testLog()
{
    TSLogger::globalLogger->init("log.log", true, TSLogger::WARN);
//    LOG_ERROR("dima ", " dddd")
}

int testServer(){
    try {
        TSLogger::globalLogger->init("log.log", true, TSLogger::TRACE);
        MsgBuilder builder(BufferPool::bufferPool);
        Chain chain(8081, 8082, &builder);

        chain.runThreads();
        sleep(2);

        boost::asio::io_service pIo_service;
        string pHost = "127.0.0.1";
        string pPort = "8082";
        Client postClient(pIo_service, pHost, pPort);


        boost::asio::io_service rIo_service;
        string rHost = "127.0.0.1";
        string rPort = "8081";
        Client recvClient(rIo_service, rHost, rPort);

        thread pClient(boost::bind(&Client::run, &postClient));
        thread rClient(boost::bind(&Client::run, &recvClient));


        recvClient.send(builder.buildListenRegistrationMsg("zzzz"));
        sleep(1);
        postClient.send(builder.buildPostRegistrationMsg("zzzz"));
        postClient.send(builder.buildPostRegistrationMsg("kkkk"));
        postClient.send(builder.buildPostPostChannelsResMsg("zzzz", "dima dima"));
        recvClient.recieve()->print_hex_memory();
        while (true) {

            postClient.send(builder.buildPostListChannelsReqMsg());
            BufferPool::bufferPool->release(postClient.recieve());
            sleep(1);
            cout << "\n";
        }
//        postClient.recieve()->print_hex_memory();
//        postClient.recieve()->print_hex_memory();
//        postClient.recieve()->print_hex_memory();
//        postClient.recieve()->print_hex_memory();
//        sleep(50);


    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


int main() {
    testServer();
    return 0;
}