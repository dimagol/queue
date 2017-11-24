#include <iostream>
#include "Server/server.h"

int smain1221()
{
    try
    {
        boost::asio::io_service io_service;
//        io_service.
        tcp_server server(io_service);

//        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


int main() {
    smain1221();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}


