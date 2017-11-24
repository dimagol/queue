//
// Created by dima on 24/11/17.
//

#include "GlobalConfig.h"

Config *GlobalConfig::conf = nullptr;
mutex *GlobalConfig::lock  = new mutex();

Config* GlobalConfig::getGlobalConfig() {
    if(conf == nullptr){
        lock->lock();
        if(conf == nullptr){
            conf = new Config("shmafka.ini");
        }
        lock->unlock();

    }
    return conf;
}
