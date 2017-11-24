#include <iostream>
#include "Server/server.h"

int smain1221()
{

}


int main() {
    try
    {
        boost::asio::io_service io_service;
        TcpServer server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


