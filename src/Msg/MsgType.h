//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_MSGTYPE_H
#define TCP_SHMAFKA_MSGTYPE_H

enum MsgType {
    UNDEFINED = -1,
    DISCONNECT_FROM_SERVER = 1,

    POST_MSG_START_ =           500,
    POST_REGISTER =             501,
    POST_POST =                 502,
    POST_DEREGISTER =           503,
    POST_DEREGISTER_ALL =       504,
    POST_DISCONNECT =           505,
    POST_LIST_CHANELES_REQ =    506,
    POST_LIST_CHANELES_RES =    507,

    LISTEN_MSG_START=           1000,
    LISTEN_REGISTER =           1001,
    LISTEN_POST =               1002,
    LISTEN_DEREGISTER =         1003,
    LISTEN_DEREGISTER_ALL =     1004,
    LISTEN_DISCONNECT =         1005,
    LISTEN_LIST_CHANELES_REQ =  1006,
    LISTEN_LIST_CHANELES_RES =  1007,


};
#endif //TCP_SHMAFKA_MSGTYPE_H
