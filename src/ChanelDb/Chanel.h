//
// Created by dima on 02/12/17.
//

#ifndef TCP_SHMAFKA_CHANEL_H
#define TCP_SHMAFKA_CHANEL_H

#include <unordered_map>
#include <unordered_set>
#include "../Processor/ProceededMsg.h"
#include "../Logging/TSLogger.h"

using namespace std;
class FullMsgBuffersContainer{
public:
    FullMsgBuffersContainer() {}

    FullMsgBuffersContainer(ProceededMsg &proceededMsg)
            : head(proceededMsg.getSocketProtoBuffer()),
              tail(proceededMsg.getSocketProtoBuffer()),
              currentLen(1),
              totalLen(proceededMsg.getNumOfChunks()){}

    bool append(ProceededMsg &proceededMsg){
        currentLen++;
        if(__glibc_unlikely(proceededMsg.getChunk() != currentLen)){
            LOG_ERROR("unable process buffer, chunk:",proceededMsg.getChunk()," currentLen:", currentLen);
            return false;
        }

        if(__glibc_unlikely(proceededMsg.getChunk() > totalLen)){
            LOG_ERROR("unable process buffer, chunk:",proceededMsg.getChunk()," currentLen:", currentLen);
            return false;
        }
        tail->nextBuffer = proceededMsg.getSocketProtoBuffer();
        tail = proceededMsg.getSocketProtoBuffer();
        return true;
    }

    bool isDone(){
        return currentLen == totalLen;
    }

    SocketProtoBuffer * head;
    SocketProtoBuffer * tail;
    uint32_t currentLen;
    uint32_t totalLen;
};
class Chanel {
public:
    Chanel(const string &chanelName) : chanelName(chanelName) {}
    void feed(ProceededMsg &proceededMsg){
        switch (proceededMsg.getType()){
            case POST:
                handlePost(proceededMsg);
                break;
            case REGISTER:
                handleRegister(proceededMsg);
                break;
            case DEREGISTER:
                handleDeregister(proceededMsg);
                break;
        }
    }

    bool isHaveNew() const {
        return haveNew;
    }

    const unordered_set<uint32_t> getRegisteredUsers() const {
        return unordered_set<uint32_t>(registeredUsers);
    }

    const pair<uint32_t, FullMsgBuffersContainer> getBuff_done() const {
        return pair<uint32_t, FullMsgBuffersContainer>(buff_done);
    }

private:
    void handlePost(ProceededMsg &proceededMsg){
        haveNew = false;
        if(proceededMsg.getChunk() != 1){
            auto buffCont = userWritingMap.find(proceededMsg.getSender_id());
            if(buffCont == userWritingMap.end()){
                LOG_ERROR("reorder event");
                return;
            }
            if (!buffCont->second.append(proceededMsg)){
                userWritingMap.erase(proceededMsg.getSender_id());
                LOG_ERROR("cant append");
                return;
            }
            if(buffCont->second.isDone()){
                buff_done.second = buffCont->second;
                buff_done.first = buffCont->first;
                userWritingMap.erase(buffCont);
                haveNew = true;
            }

        } else{
            auto buffCont = userWritingMap.insert(pair<uint32_t , FullMsgBuffersContainer>(proceededMsg.getSender_id(),proceededMsg));
            if(buffCont.first->second.isDone()){
                buff_done.first = buffCont.first->first;
                buff_done.second = buffCont.first->second;
                userWritingMap.erase(buffCont.first);
                haveNew = true;
            }
        }
        registeredUsers.insert(proceededMsg.getSender_id());
    }

    void handleRegister(ProceededMsg &proceededMsg){
        registeredUsers.insert(proceededMsg.getSender_id());
    }

    void handleDeregister(ProceededMsg &proceededMsg){
        registeredUsers.erase(proceededMsg.getSender_id());
        userWritingMap.erase(proceededMsg.getSender_id());
    }
private:
    const string chanelName;
    bool haveNew = false;


private:
    unordered_map<uint32_t , FullMsgBuffersContainer> userWritingMap;
    pair<uint32_t , FullMsgBuffersContainer> buff_done;


private:
    unordered_set<uint32_t> registeredUsers;

};


#endif //TCP_SHMAFKA_CHANEL_H
