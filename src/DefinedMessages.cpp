//
// Created by dima on 13/10/17.
//

#include <netinet/in.h>
#include <cstring>
#include "DefinedMessages.h"

Buffer * generate_msg(char * in){
    uint32_t len = (uint32_t)strlen(in);
    auto buff = new Buffer(len + 1 + 4);
    (*((uint32_t * )buff->buff)) = ntohl(len);
    memcpy(buff->buff + 4, in, len);
    return buff;

}

Buffer * DefinedMessages::hello_msg = generate_msg("Welcome to server\n");
