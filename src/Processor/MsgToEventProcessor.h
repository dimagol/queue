//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_PROCESSOR_H
#define TCP_SHMAFKA_PROCESSOR_H


#include "ProceededEvent.h"
#include "../Msg/MsgConsts.h"

class MsgToEventProcessor {
public:
//    ProceededEvent getEventByBuff(TcpServerIncomeMessage &incomeMessage){
//        switch (incomeMessage.getBuffer()->get_int(CONSTS_MSG_TYPE_OFFSET)){
//            case MsgType::POST_REGISTER:
//                return handleRegisterPost(incomeMessage);
//            case MsgType::POST_POST:
//                return handlePost(incomeMessage);
//            case MsgType::POST_DEREGISTER:
//                return handleDeregisterPost(incomeMessage);
//            case MsgType::POST_DEREGISTER_ALL:
//                return handleDeregisterAllPost(incomeMessage);
//            case MsgType::POST_LIST_CHANELES_REQ:
//                return handleListChanelsPost(incomeMessage);
//
//            case MsgType::POST_DISCONNECT:
//                return handleDisconectPost(incomeMessage);
//
//            case MsgType::LISTEN_REGISTER:
//                return handleRegisterListen(incomeMessage);
//            case MsgType::LISTEN_DEREGISTER:
//                return handleDeregisterListen(incomeMessage);
//            case MsgType::LISTEN_DEREGISTER_ALL:
//                return handleDeregisterListenAll(incomeMessage);
//            case MsgType::LISTEN_LIST_CHANELES_REQ:
//                return handleListChanelsListen(incomeMessage);
//            case LISTEN_DISCONNECT:
//                return handleDisconectListen(incomeMessage);
//
//
//            case MsgType::DISCONNECT_FROM_SERVER:
//                return handleDisconnect(incomeMessage);
//
//            default:
//                incomeMessage.getBuffer()->print_hex_memory();
//                return ProceededEvent(MsgType::UNDEFINED,incomeMessage.getId());
//        }
//    }


private:
//    ProceededEvent handleRegisterPost(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        auto ret = ProceededEvent(buff->get_string(CONSTS_POST_REGISTER_CHANEL_OFFSET),
//                            MsgType::POST_REGISTER,
//                            incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(buff);
//        return ret;
//
//
//
//    }
//    ProceededEvent handlePost(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        auto ret = ProceededEvent(buff->get_string(CONSTS_POST_CHANEL_OFFSET),
//                            MsgType::POST_POST,
//                            buff,
//                            buff->get_int(CONSTS_POST_MSG_CHUNK_OFFSET),
//                            buff->get_int(CONSTS_POST_MSG_NUM_OF_CHUNKS_OFFSET),
//                            incomeMessage.getId());
//        return ret;
//
//    }
//    ProceededEvent handleDeregisterPost(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        auto ret = ProceededEvent(buff->get_string(CONSTS_POST_DEREGISTER_CHANEL_OFFSET),
//                            MsgType::POST_DEREGISTER,
//                            incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(buff);
//        return ret;
//
//    }
//    ProceededEvent handleDeregisterAllPost(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        BufferPool::bufferPool->releaseAllChain(buff);
//        return ProceededEvent(MsgType::POST_DEREGISTER_ALL, incomeMessage.getId());
//    }
//
//    ProceededEvent handleRegisterListen(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        auto ret = ProceededEvent(buff->get_string(CONSTS_LISTEN_REGISTER_CHANEL_OFFSET),
//                            MsgType::LISTEN_REGISTER,
//                            incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(buff);
//        return ret;
//    }
//    ProceededEvent handleDeregisterListen(TcpServerIncomeMessage &incomeMessage){
//        auto buff = incomeMessage.getBuffer();
//        auto ret = ProceededEvent(buff->get_string(CONSTS_LISTEN_DEREGISTER_CHANEL_OFFSET),
//                            MsgType::LISTEN_DEREGISTER,
//                            incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(buff);
//        return ret;
//    }
//    ProceededEvent handleDeregisterListenAll(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::LISTEN_DEREGISTER_ALL, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
//    ProceededEvent handleDisconnect(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::DISCONNECT_FROM_SERVER, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
//
//
//    ProceededEvent handleListChanelsPost(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::POST_LIST_CHANELES_REQ, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
//
//    ProceededEvent handleListChanelsListen(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::LISTEN_LIST_CHANELES_REQ, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
//
//    ProceededEvent handleDisconectPost(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::POST_DISCONNECT, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
//
//    ProceededEvent handleDisconectListen(TcpServerIncomeMessage &incomeMessage){
//        auto ret =  ProceededEvent(MsgType::LISTEN_DISCONNECT, incomeMessage.getId());
//        BufferPool::bufferPool->releaseAllChain(incomeMessage.getBuffer());
//        return ret;
//    }
};


#endif //TCP_SHMAFKA_PROCESSOR_H
