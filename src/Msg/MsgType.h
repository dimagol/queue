//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_MSGTYPE_H
#define TCP_SHMAFKA_MSGTYPE_H

enum MsgType {
    UNDEFINED = -1,

    POST_MSG_START_ =           0,
    POST_REGISTER =             1,
    POST =                      2,
    POST_DEREGISTER =           3,
    POST_DEREGISTER_ALL =       4,
    POST_DISCONNECT =           5,
    POST_LIST_CHANELES_REQ =    6,
    POST_LIST_CHANELES_RES =    7,

    LISTEN_MSG_START=           1000,
    LISTEN_REGISTER =           1001,
    LISTEN_DEREGISTER =         1002,
    LISTEN_DEREGISTER_ALL =     1003,
    LISTEN_DISCONNECT =         1004,
    LISTEN_LIST_CHANELES_REQ =  1005,
    LISTEN_LIST_CHANELES_RES =  1006,


};
#endif //TCP_SHMAFKA_MSGTYPE_H
