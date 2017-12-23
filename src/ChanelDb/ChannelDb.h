//
// Created by dima on 02/12/17.
//

#ifndef TCP_SHMAFKA_CHANELDB_H
#define TCP_SHMAFKA_CHANELDB_H

#include <unordered_map>
#include "Channel.h"

using namespace std;
class ChannelDb {
public:
    bool channelExist(string &name){
        return channelMap.find(name) != channelMap.end();
    }

    shared_ptr<Channel> getChannel(const string &name){
        auto channel  = channelMap.find(name);
        if(channel == channelMap.end()){
            return nullptr;
        }
        return channel->second;
    };

    shared_ptr<Channel> createChannel(const string &name){
        auto fromMap = channelMap.insert(make_pair(name,make_shared<Channel>(name)));
        return fromMap.first->second;
    }

    bool removeChannel(string &name){
        return channelMap.erase(name) != 1;
    }

    shared_ptr<vector<shared_ptr<Channel>>> getAllPostChannelsForId(uint32_t id){
        auto ret = make_shared<vector<shared_ptr<Channel>>>();
        for(auto channel : channelMap){
            if(channel.second->containsPoster(id)){
             ret->push_back(channel.second);
            }
        }
        return ret;
    }

    shared_ptr<vector<shared_ptr<Channel>>> getAllListenChannelsForId(uint32_t id){
        auto ret = make_shared<vector<shared_ptr<Channel>>>();
        for(auto channel : channelMap){
            if(channel.second->containsListener(id)){
                ret->push_back(channel.second);
            }
        }
        return ret;
    }

    const string getChannelListStr(){
        std::stringstream ss;
        for(auto channel : channelMap){
                ss << channel.first << endl;
        }
        return ss.str();
    }


private:
    unordered_map<string,shared_ptr<Channel>> channelMap;

};


#endif //TCP_SHMAFKA_CHANELDB_H
