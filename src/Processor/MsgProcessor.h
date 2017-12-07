//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_PROCESSOR_H
#define TCP_SHMAFKA_PROCESSOR_H


#include "ProceededMsg.h"
#include "../Server/TcpServerIncomeMessage.h"
#include "../Msg/MsgConsts.h"

class MsgProcessor {
public:
    ProceededMsg processBuff(TcpServerIncomeMessage &incomeMessage){
        switch (incomeMessage.getBuffer()->get_int(CONSTS_MSG_TYPE_OFFSET)){
            case MsgType::POST:
                return handlePost(incomeMessage);
            case MsgType::REGISTER:
                return handleRegister(incomeMessage);
            case MsgType::DEREGISTER:
                return handleDeregister(incomeMessage);
            case MsgType::DISCONNECT:
                return handleDisconnect(incomeMessage);
            case MsgType::LIST_CHANELES:
                return handleListChanels(incomeMessage);
            default:
                break;
        }
    }

private:
    ProceededMsg handlePost(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededMsg(buff->get_string(CONSTS_POST_CHANEL_OFFSET),
                            MsgType::POST,
                            buff,
                            buff->get_int(CONSTS_POST_MSG_CHUNK_OFFSET),
                            buff->get_int(CONSTS_POST_MSG_NUM_OF_CHUNKS_OFFSET),
                            incomeMessage.getId(),
                            incomeMessage.getTcpServerConnectionPointer());

    }
    ProceededMsg handleRegister(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededMsg(buff->get_string(CONSTS_REGISTER_CHANEL_OFFSET),
                            MsgType::REGISTER,
                            incomeMessage.getId(),
                            incomeMessage.getTcpServerConnectionPointer());
    }
    ProceededMsg handleDeregister(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededMsg(buff->get_string(CONSTS_DEREGISTER_CHANEL_OFFSET),
                            MsgType::DEREGISTER,
                            incomeMessage.getId(),
                            incomeMessage.getTcpServerConnectionPointer());
    }
    ProceededMsg handleDisconnect(const TcpServerIncomeMessage &incomeMessage){
        return ProceededMsg(MsgType::DISCONNECT, incomeMessage.getId(), incomeMessage.getTcpServerConnectionPointer());
    }

    ProceededMsg handleListChanels(const TcpServerIncomeMessage &incomeMessage){
        return ProceededMsg(MsgType::LIST_CHANELES, incomeMessage.getId(), incomeMessage.getTcpServerConnectionPointer());
    }
};


#endif //TCP_SHMAFKA_PROCESSOR_H
