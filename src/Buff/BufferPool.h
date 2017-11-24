//
// Created by dima on 13/10/17.
//

#include <cstdint>
#include <vector>
#include <netinet/in.h>
#include <mutex>
#include <cstring>

#ifndef TCP_SHMAFKA_BUFFERPOLL_H
#define TCP_SHMAFKA_BUFFERPOLL_H
using namespace std;

#define MSG_LEN_BUFF_LEN 4

class SocketProtoBuffer{
public:
    SocketProtoBuffer(uint32_t len) : len(len) , offset(0){
        all_data = new uint8_t[len + MSG_LEN_BUFF_LEN];
        msg_len_buff = all_data;
        msg_data_buff = all_data + MSG_LEN_BUFF_LEN;

    }

    uint32_t get_msg_len(){
        return ntohl(*((uint32_t * )msg_len_buff));
    }

    void set_msg_len(uint32_t len){
        *((uint32_t *)msg_len_buff) = htonl(len);
    }

    void set_data(const uint8_t * data, uint32_t len){
        memcpy(msg_data_buff, data, len);
        *((uint32_t *)msg_len_buff) = htonl(len);
    }

    virtual ~SocketProtoBuffer() {
        delete(msg_data_buff);
    }
    uint8_t * all_data;
    uint8_t * msg_len_buff;
    uint8_t * msg_data_buff;
    uint32_t len;
private:
    uint32_t offset;
};


class BufferPool{
public:
    BufferPool(uint32_t total, uint32_t bufferLen);

    SocketProtoBuffer* get();
    void release(SocketProtoBuffer *);

    virtual ~BufferPool() {

    }

    static BufferPool* create(uint32_t total, uint32_t bufferLen);
    static BufferPool* bufferPool;
private:
    mutex lock;
    vector<SocketProtoBuffer *> bufferVector;
    uint32_t total;
    uint32_t bufferLen;
public:
};

#endif //TCP_SHMAFKA_BUFFERPOLL_H