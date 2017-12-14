//
// Created by dima on 14/12/17.
//

#ifndef TCP_SHMAFKA_SOCKETPROTOBUFFER_H
#define TCP_SHMAFKA_SOCKETPROTOBUFFER_H
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <cstring>

using namespace std;

#define MSG_LEN_BUFF_LEN 4

class SocketProtoBuffer{
public:
    explicit SocketProtoBuffer(uint32_t len) :
            len(len),
            nextBuffer(nullptr),
            sendingRefCount(0){
        all_data = new uint8_t[len + MSG_LEN_BUFF_LEN];
        msg_len_buff = all_data;
        msg_data_buff = all_data + MSG_LEN_BUFF_LEN;
    }

    const string get_string(uint32_t offset) const {
        return string((char *)msg_data_buff + offset);
    }

    const uint32_t get_int(uint32_t offset) const {
        return ntohl(*((uint32_t * )(msg_data_buff + offset)));
    }

    void print_buff() {
        cout << "all_data " << " msg_data_buff: " << msg_data_buff << " len: " << get_msg_len() << endl;
    }

    bool set_by_str(string &s){
        set_msg_len(static_cast<uint32_t>(s.size() + 1));
        set_data((const uint8_t *)(s.c_str()), static_cast<uint32_t>(s.size() + 1));
    }

    uint32_t get_msg_len(){
        return ntohl(*((uint32_t * )msg_len_buff));
    }

    uint32_t get_msg_all_data_len(){
        return ntohl(*((uint32_t * )msg_len_buff)) + MSG_LEN_BUFF_LEN;
    }

    void set_msg_len(uint32_t len){
        *((uint32_t *)msg_len_buff) = htonl(len);
    }

    void set_data(const uint8_t * data, uint32_t len){
        memcpy(msg_data_buff, data, len);
        *((uint32_t *)msg_len_buff) = htonl(len);
    }

    void append_data(const uint8_t * data, uint32_t data_len, uint32_t offset){
        memcpy(msg_data_buff + offset, data, data_len);
        *((uint32_t *)msg_len_buff) = htonl(offset + data_len);
    }

    void append_int(uint32_t data, uint32_t offset){
        data = htonl(data);
        memcpy(msg_data_buff + offset, (uint8_t *)(&data), 4);
        *((uint32_t *)msg_len_buff) = htonl(offset + 4);
    }

    void reset(){
        sendingRefCount = 0;
        nextBuffer = nullptr;

    }

    inline void incRef(){
        sendingRefCount++;
    }

    inline void decRef(){
        sendingRefCount--;
    }

    inline bool isZeroRef(){
        return sendingRefCount == 0;
    }

    inline void setRefCountList(uint32_t refCount){
        for (auto buff = this ; buff != nullptr; buff=buff->nextBuffer){
            buff->sendingRefCount = refCount;
        }
    }

    virtual ~SocketProtoBuffer() {
        delete(msg_data_buff);
    }
    uint8_t * all_data;
    uint8_t * msg_len_buff;
    uint8_t * msg_data_buff;
    uint32_t len;
    SocketProtoBuffer * nextBuffer;
    uint32_t sendingRefCount ;

};




#endif //TCP_SHMAFKA_SOCKETPROTOBUFFER_H
