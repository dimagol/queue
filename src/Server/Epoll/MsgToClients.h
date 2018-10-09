//
// Created by dima on 08/10/18.
//

#ifndef TCP_SHMAFKA_MSGTOCLIENTS_H
#define TCP_SHMAFKA_MSGTOCLIENTS_H


#include <set>
#include <bits/unordered_set.h>
#include "../../Buff/ThreadSafeBufferList.h"

class MsgToClients {
    BuffersList bufferList;
    unordered_set<uint32_t> clientIdSet;

};


#endif //TCP_SHMAFKA_MSGTOCLIENTS_H
