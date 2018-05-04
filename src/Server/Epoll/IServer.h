//
// Created by dima on 27/04/18.
//

#ifndef TCP_SHMAFKA_ISERVEREVENTSLISTENER_H
#define TCP_SHMAFKA_ISERVEREVENTSLISTENER_H

#include <cstdint>
#include "Client.h"
#include "../../Buff/SocketProtoBuffer.h"

class IServer
{
    virtual void onNewConnection(Client &client) = 0;

    virtual void onConnectionClose(Client &client) = 0;

    virtual void onBufferFromClient(Client &client, SocketProtoBuffer &buffer) = 0;

    virtual void writeBufferToClient(Client &client, SocketProtoBuffer &buffer) = 0;

    virtual void disconnectClient(Client &client) = 0;



};


#endif //TCP_SHMAFKA_ISERVEREVENTSLISTENER_H
