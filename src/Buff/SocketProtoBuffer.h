//

// Created by dima on 14/12/17.

//
#ifndef TCP_SHMAFKA_SOCKETPROTOBUFFER_H
#define TCP_SHMAFKA_SOCKETPROTOBUFFER_H
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sstream>

using namespace std;

#define MSG_LEN_BUFF_LEN 4

class SocketProtoBuffer{
public:
    explicit SocketProtoBuffer(uint32_t len) :
            len(len),
            nextBuffer(nullptr),
            msg_complete_buff(new uint8_t[len]),
            wasCopiedOrMoved(false),
            offset(0){}

    // copy constructor
    SocketProtoBuffer (SocketProtoBuffer &obj):
            len(obj.len),
            nextBuffer(obj.nextBuffer),
            msg_complete_buff(obj.msg_complete_buff),
            wasCopiedOrMoved(false),
            offset(obj.offset)
    {
        obj.wasCopiedOrMoved = true;
    }


    // move constructor
    SocketProtoBuffer (SocketProtoBuffer &&obj) noexcept:
            len(obj.len),
            nextBuffer(obj.nextBuffer),
            msg_complete_buff(obj.msg_complete_buff),
            wasCopiedOrMoved(false),
            offset(obj.offset){
        obj.wasCopiedOrMoved = true;
    }

    inline const string get_string(uint32_t offset) const;

    inline const uint32_t get_int(uint32_t offset) const;

    inline uint32_t get_msg_len() const ;

    inline void set_data(uint8_t* data, uint32_t offset, uint32_t len);

    inline void set_msg_len(uint32_t len);

    inline void set_int(uint32_t data, uint32_t offset);

    inline void reset();

    inline void print_hex_memory() const ;

    inline virtual ~SocketProtoBuffer(){
        if(!wasCopiedOrMoved){
            delete(msg_complete_buff);
        }
    }

    uint8_t * const msg_complete_buff;

    uint32_t offset;


    SocketProtoBuffer * nextBuffer;
    SocketProtoBuffer * lastBuff{};

    bool wasCopiedOrMoved;
    const uint32_t len;

};

const string SocketProtoBuffer::get_string(uint32_t offset) const {
    return string((char *)msg_complete_buff + offset);
}

const uint32_t SocketProtoBuffer::get_int(uint32_t offset) const {
    return ntohl(*((uint32_t * )(msg_complete_buff + offset)));
}

void SocketProtoBuffer::print_hex_memory() const {

    int all_msg_len = get_msg_len();
    if (all_msg_len > len)
    {
        all_msg_len=len;
    }
    int num_of_lines = all_msg_len / 16;
    int res = all_msg_len % 16;
    int index = 0;
    stringstream ss("");
    cout << "buff len : " << all_msg_len << endl;
    for (int i = 0 ; i < num_of_lines ; i++){
        ss << std::hex << (int)msg_complete_buff[index] << " " << std::hex << (int)msg_complete_buff[index +1] << " "
           << std::hex << (int)msg_complete_buff[index +2] << " " << std::hex << (int)msg_complete_buff[index +3] << " "
           << std::hex << (int)msg_complete_buff[index +4] << " " << std::hex << (int)msg_complete_buff[index +5] << " "
           << std::hex << (int)msg_complete_buff[index +6] << " " << std::hex << (int)msg_complete_buff[index +7] << " "
           << std::hex << (int)msg_complete_buff[index +8] << " " << std::hex << (int)msg_complete_buff[index +9] << " "
           << std::hex << (int)msg_complete_buff[index +10] << " " << std::hex << (int)msg_complete_buff[index +11] << " "
           << std::hex << (int)msg_complete_buff[index +12] << " " << std::hex << (int)msg_complete_buff[index +13] << " "
           << std::hex << (int)msg_complete_buff[index +14] << " " << std::hex << (int)msg_complete_buff[index +15] << "         "
           << (char)msg_complete_buff[index] << " " << (char)msg_complete_buff[index +1] << " "
           << (char)msg_complete_buff[index +2] << " " << (char)msg_complete_buff[index +3] << " "
           << (char)msg_complete_buff[index +4] << " " << (char)msg_complete_buff[index +5] << " "
           << (char)msg_complete_buff[index +6] << " " << (char)msg_complete_buff[index +7] << " "
           << (char)msg_complete_buff[index +8] << " " << (char)msg_complete_buff[index +9] << " "
           << (char)msg_complete_buff[index +10] << " " << (char)msg_complete_buff[index +11] << " "
           << (char)msg_complete_buff[index +12] << " " << (char)msg_complete_buff[index +13] << " "
           << (char)msg_complete_buff[index +14] << " " << (char)msg_complete_buff[index +15] << endl;

//        printf("%02x %02x %02x %02x "
//                       "%02x %02x %02x %02x "
//                       "%02x %02x %02x %02x "
//                       "%02x %02x %02x %02x         "
//                       "%c %0c %0c %0c "
//                       "%c %0c %0c %0c "
//                       "%c %0c %0c %0c "
//                       "%c %0c %0c %0c \n"
//                , msg_complete_buff[index], msg_complete_buff[index+1],msg_complete_buff[index+2],msg_complete_buff[index+3]
//                , msg_complete_buff[index+4], msg_complete_buff[index+5],msg_complete_buff[index+6],msg_complete_buff[index+7]
//                , msg_complete_buff[index+8], msg_complete_buff[index+9],msg_complete_buff[index+10],msg_complete_buff[index+11]
//                , msg_complete_buff[index+12], msg_complete_buff[index+13],msg_complete_buff[index+14],msg_complete_buff[index+15]
//
//                , (char)msg_complete_buff[index], (char)msg_complete_buff[index+1],(char)msg_complete_buff[index+2],(char)msg_complete_buff[index+3]
//                , (char)msg_complete_buff[index+4], (char)msg_complete_buff[index+5],(char)msg_complete_buff[index+6],(char)msg_complete_buff[index+7]
//                , (char)msg_complete_buff[index+8], (char)msg_complete_buff[index+9],(char)msg_complete_buff[index+10],(char)msg_complete_buff[index+11]
//                , (char)msg_complete_buff[index+12], (char)msg_complete_buff[index+13],(char)msg_complete_buff[index+14],(char)msg_complete_buff[index+15]);
        index+=16;
    }

    for (int i = 0 ; i < res ; i++){
        ss << hex << (int)msg_complete_buff[index+i] << " ";
//        printf("%02x ", msg_complete_buff[index+i]);
    }
    for (int i = 0 ; i < 16-res ; i++){
        ss << "   ";

    }
    ss << "       ";
    for (int i = 0 ; i < res ; i++){
        ss << " " << (char)msg_complete_buff[index+i];
//        printf(" %c", (char)msg_complete_buff[index+i]);
    }
    cout << ss.str() << endl;
}

uint32_t SocketProtoBuffer::get_msg_len() const{
    return ntohl(*((uint32_t * )msg_complete_buff));
}

void SocketProtoBuffer::set_msg_len(uint32_t len) {
    *((uint32_t *)msg_complete_buff) = htonl(len);
}


void SocketProtoBuffer::set_int(uint32_t data, uint32_t offset) {
    data = htonl(data);
    memcpy(msg_complete_buff + offset, (uint8_t *)(&data), 4);
}

void SocketProtoBuffer::set_data(uint8_t *data, uint32_t offset, uint32_t len) {
    memcpy(msg_complete_buff + offset, data, len);
}

void SocketProtoBuffer::reset() {
    nextBuffer = nullptr;
    offset = 0;

}





#endif //TCP_SHMAFKA_SOCKETPROTOBUFFER_H
