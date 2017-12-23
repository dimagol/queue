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
    void print_hex_memory() {
        int all_msg_len = get_msg_len() + 4;
        int num_of_lines = all_msg_len / 16;
        int res = all_msg_len % 16;
        int index = 0;
        cout << "buff len : " << all_msg_len << endl;
        for (int i = 0 ; i < num_of_lines ; i++){
                printf("%02x %02x %02x %02x "
                       "%02x %02x %02x %02x "
                       "%02x %02x %02x %02x "
                       "%02x %02x %02x %02x         "
                               "%c %0c %0c %0c "
                               "%c %0c %0c %0c "
                               "%c %0c %0c %0c "
                               "%c %0c %0c %0c \n"
                        , all_data[index], all_data[index+1],all_data[index+2],all_data[index+3]
                        , all_data[index+4], all_data[index+5],all_data[index+6],all_data[index+7]
                        , all_data[index+8], all_data[index+9],all_data[index+10],all_data[index+11]
                        , all_data[index+12], all_data[index+13],all_data[index+14],all_data[index+15]

                        , (char)all_data[index], (char)all_data[index+1],(char)all_data[index+2],(char)all_data[index+3]
                        , (char)all_data[index+4], (char)all_data[index+5],(char)all_data[index+6],(char)all_data[index+7]
                        , (char)all_data[index+8], (char)all_data[index+9],(char)all_data[index+10],(char)all_data[index+11]
                        , (char)all_data[index+12], (char)all_data[index+13],(char)all_data[index+14],(char)all_data[index+15]);
                index+=16;
        }

        for (int i = 0 ; i < res ; i++){
            printf("%02x ", all_data[index+i]);
        }
        for (int i = 0 ; i < 16-res ; i++){
            printf("   ");

        }
        printf("       ");
        for (int i = 0 ; i < res ; i++){
            printf(" %c", (char)all_data[index+i]);
        }
        printf("\n");
    }

    bool set_by_str(string &s, uint32_t type){
        append_int(type,0);
        append_data((const uint8_t *)(s.c_str()), s.size() + 1, 4);
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

    void set_int(uint32_t data, uint32_t offset){
        data = htonl(data);
        memcpy(msg_data_buff + offset, (uint8_t *)(&data), 4);
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
