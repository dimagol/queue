//
// Created by dima on 24/11/17.
//

#include "GlobalConfig.h"

GlobalConfig::conf = nullptr;
GlobalConfig::lock;

Config* GlobalConfig::getGlobalConfig() {
    if(conf == nullptr){
        lock.lock();
        if(conf == nullptr){
            conf = new Config("shmafka.ini");
        }
        lock.unlock();

    }
    return conf;
}
