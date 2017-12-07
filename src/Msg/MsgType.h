//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_MSGTYPE_H
#define TCP_SHMAFKA_MSGTYPE_H

enum MsgType {
    POST = 1,
    REGISTER = 2,
    DEREGISTER = 3,
    DISCONNECT = 4,
    LIST_CHANELES = 5
};
#endif //TCP_SHMAFKA_MSGTYPE_H
