//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_MSGTYPE_H
#define TCP_SHMAFKA_MSGTYPE_H

enum MsgType {
    UNDEFINED = -1,

    POST_REGISTER =             1,
    POST =                      2,
    POST_DEREGISTER =           3,
    POST_DEREGISTER_ALL =       4,
    POST_DISCONNECT =           5,

    LISTEN_REGISTER =           6,
    LISTEN_DEREGISTER =         7,
    LISTEN_DEREGISTER_ALL =     8,
    LISTEN_DISCONNECT =         9,


    LIST_CHANELES =             10
};
#endif //TCP_SHMAFKA_MSGTYPE_H
