//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_USERDATA_H
#define TCP_SHMAFKA_USERDATA_H


#include "../../Buff/ThreadSafeBufferList.h"
#include "../../Buff/ThreadSafeListOfBuffersLists.h"

class UserData {
public:
    UserData() {
        sendListLsits = new ThreadSafeListOfBuffersLists();
        recvList = new ThreadSafeBufferList();
    }

    virtual ~UserData(){
        delete sendListLsits;
        delete recvList;
    }

public:
    int fd = -1;
    ThreadSafeListOfBuffersLists * sendListLsits= nullptr;
    ThreadSafeBufferList * recvList = nullptr;
};



#endif //TCP_SHMAFKA_USERDATA_H
