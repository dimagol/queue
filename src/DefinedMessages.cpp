//
// Created by dima on 13/10/17.
//

#include <netinet/in.h>
#include <cstring>
#include "DefinedMessages.h"

SocketProtoBuffer * generate_str_msg(const char *in){
    uint32_t len = (uint32_t)strlen(in);
    auto buff = new SocketProtoBuffer(len + MSG_LEN_BUFF_LEN + 1);
    buff->set_msg_len(len +1);
    buff->set_data((const uint8_t *)in, len + 1);
    return buff;

}
const char welcome[] = "Welcome to server\n";
SocketProtoBuffer * DefinedMessages::hello_msg = generate_str_msg(welcome);
