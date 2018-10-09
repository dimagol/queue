//
// Created by dima on 08/10/18.
//

#ifndef TCP_SHMAFKA_SIMPLEMSGLISTBUILDER_H
#define TCP_SHMAFKA_SIMPLEMSGLISTBUILDER_H


#include "../../Buff/ThreadSafeBufferList.h"
#include "SimpleMsgHeader.h"

class SimpleMsgListBuilder {
public:
    ThreadSafeBufferList * buildFromBuff(uint8_t * buff, uint32_t len){
        ThreadSafeBufferList * ret = new ThreadSafeBufferList();
        SocketProtoBuffer * buffer = BufferPool::bufferPool->get();

        uint32_t  dataLen = BufferPool::bufferPool->getBufferLen() - SIMPLE_MSG_CHUNK_ALL_HEADER_LEN;
        uint32_t numOfChunks = len / dataLen;
        if(len % dataLen != 0){
            numOfChunks++;
        }

        for (uint32_t chunkNum = 1; chunkNum <= numOfChunks ; chunkNum++){

            SocketProtoBuffer * socketProtoBuffer = BufferPool::bufferPool->get();

            if (socketProtoBuffer == nullptr){
                BufferPool::bufferPool->releaseAllChain(ret->get_head());

                return nullptr;
            }

            uint32_t currentDataLen;

            if(len >= dataLen){
                currentDataLen = dataLen;
            } else {
                currentDataLen = len;
            }

            socketProtoBuffer->set_msg_len(currentDataLen + SIMPLE_MSG_CHUNK_ALL_HEADER_LEN);
            socketProtoBuffer->set_int(numOfChunks, SIMPLE_MSG_NUMBER_OF_CHUNCKS_OFFSET);
            socketProtoBuffer->set_int(chunkNum, SIMPLE_MSG_CHUNK_NUMBER_OFFSET);
            socketProtoBuffer->set_data(buff, SIMPLE_MSG_DATA_OFFSET, currentDataLen);
            len -= currentDataLen;
            ret->append(socketProtoBuffer);
        }
        return ret;
    }
};


#endif //TCP_SHMAFKA_SIMPLEMSGLISTBUILDER_H
