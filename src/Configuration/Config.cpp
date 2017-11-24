//
// Created by dima on 24/11/17.
//
#include <iniparser.h>
#include "Config.h"
dictionary * dict = nullptr;
Config::Config(const string &filePath) {
    dict = iniparser_load(filePath.c_str());
}

double Config::getDouble(const string &key, double defaultVal) {
    return iniparser_getdouble(dict, key.c_str(), defaultVal);
}

string Config::getStr(const string &key, const string &defaultVal) {
    return iniparser_getstring(dict, key.c_str(), const_cast<char *>(defaultVal.c_str()));
}

uint32_t  Config::getInt(const string &key, uint32_t defaultVal) {
    return static_cast<uint32_t>(iniparser_getint(dict, key.c_str(), defaultVal));
}
