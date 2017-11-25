#include <iostream>
#include "Server/TcpServer.h"
#include "Client/Client.h"

void printit(const shared_ptr<pair<uint32_t, SocketProtoBuffer *>> &all);


int main() {
    try
    {
        boost::asio::io_service io_service_server;
        TcpServer server(io_service_server);
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

        buffer->set_by_str(str1);
        client.send(buffer);
        server.recieve()->second->print_buff();

        buffer = BufferPool::bufferPool->get();
        buffer->set_by_str(str1);
        client.send(buffer);
        server.recieve()->second->print_buff();

        buffer = BufferPool::bufferPool->get();
        buffer->set_by_str(str2);
        server.send(0,buffer);
        client.recieve()->print_buff();

        buffer = BufferPool::bufferPool->get();
        buffer->set_by_str(str1);
        client.send(buffer);
        server.recieve()->second->print_buff();


        buffer = BufferPool::bufferPool->get();
        buffer->set_by_str(str3);
        server.send(0,buffer);
        client.recieve()->print_buff();







        sleep(1000);


    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void printit(const shared_ptr<pair<uint32_t, SocketProtoBuffer *>> &all) {
//    all->second->print_buff();
//    cout << all->first << endl;
}


