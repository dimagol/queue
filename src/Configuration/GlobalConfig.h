//
// Created by dima on 24/11/17.
//

#ifndef TCP_SHMAFKA_GLOBALCONFIG_H
#define TCP_SHMAFKA_GLOBALCONFIG_H


#include "Config.h"
#include <mutex>

class GlobalConfig {
public:
    static Config * getGlobalConfig();
public:
    static mutex * lock;
    static Config * conf;
};


#endif //TCP_SHMAFKA_GLOBALCONFIG_H
