//
// Created by dima on 02/12/17.
//

#ifndef TCP_SHMAFKA_CHANEL_H
#define TCP_SHMAFKA_CHANEL_H

#include <unordered_map>
#include <unordered_set>
#include "../Processor/ProceededEvent.h"
#include "../Logging/TSLogger.h"

using namespace std;
class FullMsgBuffersContainer{
public:
    FullMsgBuffersContainer() {}

    void init(ProceededEvent &proceededMsg){
        head=proceededMsg.getSocketProtoBuffer();
        tail = head;
        currentLen=1;
        totalLen = proceededMsg.getNumOfChunks();

    }

    bool append(ProceededEvent &proceededMsg){
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
class Channel {
public:
    Channel(const string &chanelName) : chanelName(chanelName) {}

    void feed(ProceededEvent &proceededMsg){
        switch (proceededMsg.getType()){
            case MsgType::POST_REGISTER:
                return handlePostRegister(proceededMsg);
            case MsgType::POST:
                return handlePost(proceededMsg);
            case MsgType::POST_DEREGISTER:
                return handlePostDeregister(proceededMsg);
            case MsgType::POST_DEREGISTER_ALL:
                return handlePostDeregister(proceededMsg);
            case MsgType::POST_DISCONNECT:
                return handlePostDeregister(proceededMsg);

            case MsgType::LISTEN_REGISTER:
                return handleListenRegister(proceededMsg);
            case MsgType::LISTEN_DEREGISTER:
                return handleListenDeregister(proceededMsg);
            case MsgType::LISTEN_DEREGISTER_ALL:
                return handleListenDeregister(proceededMsg);
            case MsgType::LISTEN_DISCONNECT:
                return handleListenDeregister(proceededMsg);

            default:
                LOG_ERROR("unexpected msg",proceededMsg.getType())
                break;
        }
    }

    bool containsListener(uint32_t id){
        return registeredListenUsersSet.find(id) != registeredListenUsersSet.end();
    }

    bool containsPoster(uint32_t id){
        return userPostingMap.find(id) != userPostingMap.end();
    }

    bool haveNewData() const {
        return haveNew;
    }

    void setNoData(){
        haveNew = false;
    }

    const unordered_set<uint32_t> getRegisteredUsers() const {
        return unordered_set<uint32_t>(registeredListenUsersSet);
    }

    const shared_ptr<FullMsgBuffersContainer> getBuffDone() const {
        return buff_done.second;
    }

private:
    void handlePostRegister(ProceededEvent &proceededMsg){
        if(userPostingMap.find(proceededMsg.getSender_id()) != userPostingMap.end()){
            LOG_WARN("already exists " , proceededMsg.getSender_id());
        } else{
            userPostingMap.insert(make_pair<uint32_t , shared_ptr<FullMsgBuffersContainer>>(
                            proceededMsg.getSender_id(),
                            make_shared<FullMsgBuffersContainer>())).first->second->init(proceededMsg);
        }
    }
    void handlePost(ProceededEvent &proceededMsg){
        haveNew = false;
        if(proceededMsg.getChunk() != 1){
            auto buffCont = userPostingMap.find(proceededMsg.getSender_id());
            if(buffCont == userPostingMap.end()){
                LOG_ERROR("reorder event");
                return;
            }
            if (!buffCont->second->append(proceededMsg)){
                userPostingMap.erase(proceededMsg.getSender_id());
                LOG_ERROR("cant append");
                return;
            }
            if(buffCont->second->isDone()){
                buff_done.second = buffCont->second;
                buff_done.first = buffCont->first;
                userPostingMap.erase(buffCont);
                haveNew = true;
                return;
            }
            return;

        } else {
            auto fromMap = userPostingMap.find(proceededMsg.getSender_id());
            if(fromMap == userPostingMap.end()){
                LOG_ERROR("cant find buffer for ",proceededMsg.getSender_id(), " " , chanelName);
                return;
            } else{
                if (!fromMap->second->append(proceededMsg)){
                    LOG_ERROR("cant process",proceededMsg.getSender_id(), " " , chanelName);
                    return;
                }
            }

            if(fromMap->second->isDone()){
                buff_done.first = proceededMsg.getSender_id();
                buff_done.second= fromMap->second;
                userPostingMap.erase(proceededMsg.getSender_id());
                haveNew = true;
                return;
            }
            return;
        }
    }
    void handlePostDeregister(ProceededEvent &proceededMsg){
        if (userPostingMap.erase(proceededMsg.getSender_id()) != 1){
            LOG_ERROR("cant deregister")
        }
    }

    void handleListenRegister(ProceededEvent &proceededMsg){
        if(!registeredListenUsersSet.insert(proceededMsg.getSender_id()).second){
            LOG_WARN("user ",proceededMsg.getSender_id() ," already registerd ", chanelName);
        }
    }
    void handleListenDeregister(ProceededEvent &proceededMsg){
        if(registeredListenUsersSet.erase(proceededMsg.getSender_id()) != 1){
            LOG_WARN("user ",proceededMsg.getSender_id() ," already deregisterd ", chanelName);
        }
    }

private:
    const string chanelName;

    unordered_map<uint32_t , shared_ptr<FullMsgBuffersContainer>> userPostingMap;
    unordered_set<uint32_t> registeredListenUsersSet;

    bool haveNew = false;
    pair<uint32_t , shared_ptr<FullMsgBuffersContainer>> buff_done;

};


#endif //TCP_SHMAFKA_CHANEL_H
