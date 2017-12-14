#include <iostream>
#include "Server/TcpServer.h"
#include "Client/Client.h"
#include "Logging/TSLogger.h"

int testLog()
{
    globalLogger.init("log.log", true, TSLogger::WARN);
    LOG_ERROR("dima ", " dddd")
}

int testServer(){
    try
    {
        boost::asio::io_service io_service_server;
        TcpServer server(io_service_server,8081);
        thread tServer(boost::bind(&TcpServer::run, &server));
        sleep(1);



        boost::asio::io_service io_service_client;
        std::string local("localhost");
        std::string port("8081");



        Client client(io_service_client, local, port);
        thread tClient(boost::bind(&Client::run, &client));

        SocketProtoBuffer * buffer = BufferPool::bufferPool->get();
        std::string str1("kkkk");
        std::string str2("kkkk11111111");
        std::string str3("kkkk3333333");
//
//        buffer->set_by_str(str1);
//        client.send(buffer);
//        server.recieve()->second->print_buff();
//
//        buffer = BufferPool::bufferPool->getChunkedBuffer();
//        buffer->set_by_str(str1);
//        client.send(buffer);
//        server.recieve()->second->print_buff();
//
//        buffer = BufferPool::bufferPool->getChunkedBuffer();
//        buffer->set_by_str(str2);
//        server.send(0,buffer);
//        client.recieve()->print_buff();
//
//        buffer = BufferPool::bufferPool->getChunkedBuffer();
//        buffer->set_by_str(str1);
//        client.send(buffer);
//        server.recieve()->second->print_buff();
//
//
//        buffer = BufferPool::bufferPool->getChunkedBuffer();
//        buffer->set_by_str(str3);
//        server.send(0,buffer);
//        client.recieve()->print_buff();


        sleep(1000);


    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


int main() {
    shared_ptr<pair<int,int >> p = make_shared<pair<int,int >> (make_pair<int,int>(1,1));
    if(p == nullptr){
        cout << 1 << endl;
    } else{
        cout << 2 << endl;
    }
    unordered_set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    for(auto val : set){
        cout << " aaa "<<val << endl;
    }
    return 0;
}