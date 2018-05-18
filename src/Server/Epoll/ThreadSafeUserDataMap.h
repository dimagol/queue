//
// Created by dima on 18/05/18.
//

#ifndef TCP_SHMAFKA_THREADSAFEUSERDATAMAP_H
#define TCP_SHMAFKA_THREADSAFEUSERDATAMAP_H


#include <unordered_map>
#include "UserData.h"

class ThreadSafeUserDataMap {
public:

    UserData * getFromMap(uint32_t id){
        std::lock_guard<std::mutex> lock(mapLock);
        auto fromMap = clientMap.find(id);
        auto mapEnd = clientMap.end();
        if(fromMap == mapEnd){
            return nullptr;
        } else{
            return fromMap->second;
        }
    }

    bool putToMap(uint32_t id, UserData * ptr){
        std::lock_guard<std::mutex> lock(mapLock);
        bool isInMap = clientMap.find(id) != clientMap.end();
        if(!isInMap){
            clientMap.insert(make_pair(id,ptr));
        } else{
            LOG_ERROR("id ", id, " already in map")
        }
        return !isInMap;
    }

    bool removeFromMap(uint32_t id){
        std::lock_guard<std::mutex> lock(mapLock);
        bool isInMap = clientMap.find(id) != clientMap.end();
        if(isInMap){
            auto from = clientMap.find(id);
            clientMap.erase(id);
            delete(from->second);
            LOG_INFO("client ", id, " was removed")
        } else{
            LOG_ERROR("id ", id, " not in map")
        }
        return isInMap;
    }

    bool mapContains(uint32_t id){
        std::lock_guard<std::mutex> lock(mapLock);
        bool isInMap = clientMap.find(id) != clientMap.end();
        return isInMap;
    }

private:
    unordered_map<uint32_t , UserData *> clientMap;
    mutex mapLock{};
};


#endif //TCP_SHMAFKA_THREADSAFEUSERDATAMAP_H
