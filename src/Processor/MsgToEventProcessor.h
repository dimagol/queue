//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_PROCESSOR_H
#define TCP_SHMAFKA_PROCESSOR_H


#include "ProceededEvent.h"
#include "../Server/TcpServerIncomeMessage.h"
#include "../Msg/MsgConsts.h"

class MsgToEventProcessor {
public:
    ProceededEvent processBuff(TcpServerIncomeMessage &incomeMessage){
        cout << incomeMessage.getBuffer()->get_msg_len() <<endl;
        switch (incomeMessage.getBuffer()->get_int(CONSTS_MSG_TYPE_OFFSET)){
            case MsgType::POST_REGISTER:
                return handleRegisterPost(incomeMessage);
            case MsgType::POST_POST:
                return handlePost(incomeMessage);
            case MsgType::POST_DEREGISTER:
                return handleDeregisterPost(incomeMessage);
            case MsgType::POST_DEREGISTER_ALL:
                return handleDeregisterAllPost(incomeMessage);
            case MsgType::POST_DISCONNECT:
                return handleDisconnectPost(incomeMessage);
            case MsgType::POST_LIST_CHANELES_REQ:
                return handleListChanelsPost(incomeMessage);

            case MsgType::LISTEN_REGISTER:
                return handleRegisterListen(incomeMessage);
            case MsgType::LISTEN_DEREGISTER:
                return handleDeregisterListen(incomeMessage);
            case MsgType::LISTEN_DEREGISTER_ALL:
                return handleDeregisterListenAll(incomeMessage);
            case MsgType::LISTEN_DISCONNECT:
                return handleDisconnectListen(incomeMessage);
            case MsgType::LISTEN_LIST_CHANELES_REQ:
                return handleListChanelsListen(incomeMessage);

            default:
                return ProceededEvent(MsgType::UNDEFINED,incomeMessage.getId());
        }
    }


private:
    ProceededEvent handleRegisterPost(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededEvent(buff->get_string(CONSTS_POST_REGISTER_CHANEL_OFFSET),
                            MsgType::POST_REGISTER,
                            incomeMessage.getId());

    }
    ProceededEvent handlePost(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        cout << buff->get_msg_len() << endl;
        for (int i = 0 ; i < 40; i++){
            cout << (int)buff->all_data[i] << " zzzz"<< endl;
        }
        cout << buff->get_int(CONSTS_POST_MSG_CHUNK_OFFSET) << endl;
        cout << buff->get_int(CONSTS_POST_MSG_NUM_OF_CHUNKS_OFFSET) << endl;
        return ProceededEvent(buff->get_string(CONSTS_POST_CHANEL_OFFSET),
                            MsgType::POST_POST,
                            buff,
                            buff->get_int(CONSTS_POST_MSG_CHUNK_OFFSET),
                            buff->get_int(CONSTS_POST_MSG_NUM_OF_CHUNKS_OFFSET),
                            incomeMessage.getId());

    }
    ProceededEvent handleDeregisterPost(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededEvent(buff->get_string(CONSTS_POST_DEREGISTER_CHANEL_OFFSET),
                            MsgType::POST_DEREGISTER,
                            incomeMessage.getId());

    }
    ProceededEvent handleDeregisterAllPost(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::POST_DEREGISTER_ALL, incomeMessage.getId());
    }
    ProceededEvent handleDisconnectPost(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::POST_DISCONNECT, incomeMessage.getId());
    }

    ProceededEvent handleRegisterListen(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededEvent(buff->get_string(CONSTS_LISTEN_REGISTER_CHANEL_OFFSET),
                            MsgType::LISTEN_REGISTER,
                            incomeMessage.getId());
    }
    ProceededEvent handleDeregisterListen(TcpServerIncomeMessage &incomeMessage){
        auto buff = incomeMessage.getBuffer();
        return ProceededEvent(buff->get_string(CONSTS_LISTEN_DEREGISTER_CHANEL_OFFSET),
                            MsgType::LISTEN_DEREGISTER,
                            incomeMessage.getId());
    }
    ProceededEvent handleDeregisterListenAll(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::LISTEN_DEREGISTER_ALL, incomeMessage.getId());
    }
    ProceededEvent handleDisconnectListen(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::LISTEN_DISCONNECT, incomeMessage.getId());
    }


    ProceededEvent handleListChanelsPost(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::POST_LIST_CHANELES_REQ, incomeMessage.getId());
    }

    ProceededEvent handleListChanelsListen(const TcpServerIncomeMessage &incomeMessage){
        return ProceededEvent(MsgType::LISTEN_LIST_CHANELES_REQ, incomeMessage.getId());
    }
};


#endif //TCP_SHMAFKA_PROCESSOR_H
