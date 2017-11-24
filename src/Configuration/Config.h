//
// Created by dima on 24/11/17.
//

#ifndef TCP_SHMAFKA_CONFIG_H
#define TCP_SHMAFKA_CONFIG_H


#include <string>

using namespace std;

class Config {

public:
    explicit Config(const string &filePath);
    string getStr(const string &key, const string &defaultVal);
    uint32_t getInt(const string &key, uint32_t defaultVal);
    double getDouble(const string &key, double defaultVal);
};


#endif //TCP_SHMAFKA_CONFIG_H
