//
// Created by dima on 25/01/18.
//

#ifndef TCP_SHMAFKA_ARGSPARSER_H
#define TCP_SHMAFKA_ARGSPARSER_H

#include <iostream>
#include <string>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

using namespace std;

enum ArgType{
    U_INT16=1,
    U_INT32=2,
    STRING=3,
    BOOL=4
};

struct ArgValue{
    string str_val = "";
    uint32_t uint32_val = 0;
    uint16_t uint16_val = 0;
    bool bool_val = false;
};


struct Arg{
    ArgType type;
    ArgValue value;
};

struct DefinedArg{
    DefinedArg(ArgType type,
               ArgValue default_value,
               const string& name,
               bool mandatory,
               const string& help)
            : type(type),
              default_value(default_value),
              name(name),
              mandatory(mandatory),
              help(help) {}

    virtual ~DefinedArg() {

    }

    ArgType type;
    ArgValue default_value;
    const string name;
    bool mandatory;
    const string help;
};
class ArgsParser {
    map<string,DefinedArg> definedArgs;
    map<string,Arg> resultsArgs;
public:
    void addStrArg(const string &name, const string &help){
        ArgValue argValue;
        argValue.str_val = "";
        DefinedArg arg(STRING, argValue, name, true, help);
        definedArgs.insert(make_pair(name,arg));
    }
    void addStrArgWithDefault(const string &name, const string &help, const string &defaultVal){
        ArgValue argValue;
        argValue.str_val = defaultVal;
        DefinedArg arg(STRING, argValue, name, false, help);
        definedArgs.insert(make_pair(name,arg));
    }

    void addUint16Arg(const string &name, const string &help){
        ArgValue argValue;
        argValue.uint16_val = 0;
        DefinedArg arg(U_INT16, argValue, name, true, help);
        definedArgs.insert(make_pair(name,arg));
    }
    void addUint16ArgWithDefault(const string &name, const string &help, uint16_t defaultVal){
        ArgValue argValue;
        argValue.uint16_val = defaultVal;
        DefinedArg arg(U_INT16, argValue, name, false, help);
        definedArgs.insert(make_pair(name,arg));
    }

    void addUint32Arg(const string &name, const string &help){
        ArgValue argValue;
        argValue.uint32_val = 0;
        DefinedArg arg(U_INT32, argValue, name, true, help);
        definedArgs.insert(make_pair(name,arg));
    }
    void addUint32ArgWithDefault(const string &name, const string &help, uint32_t defaultVal){
        ArgValue argValue;
        argValue.uint32_val = defaultVal;
        DefinedArg arg(U_INT32, argValue, name, false, help);
        definedArgs.insert(make_pair(name,arg));
    }

    void addFlag(const string &name, const string &help){
        ArgValue argValue;
        argValue.bool_val = false;
        DefinedArg arg(BOOL, argValue, name, false, help);
        definedArgs.insert(make_pair(name,arg));
    }

    map<string,Arg>& getResults(){
        return resultsArgs;
    };

    bool generateArgs(int argc, char *argv[]){
        vector<string> argVec;
        for (int i = 0; i < argc; i++){
            argVec.emplace_back(argv[i]);
        }

        return !(!parseArgs(argVec) || !populateDefaults());

    }

    void printUsage() {
        cout << "Usage:" << endl;
        for (auto const &value: definedArgs) {
            string arg = value.first;
            DefinedArg definedArg = value.second;
            cout << "   --" << arg << " - type : ";
            switch (definedArg.type) {
                case STRING:
                    cout << "string " << endl;
                    break;
                case U_INT32:
                    cout << "32 unsigned int" << endl;
                    break;
                case U_INT16:
                    cout << "16 unsigned int" << endl;
                    break;
                case BOOL:
                    cout << "flag" << endl;
                    break;
            }
            cout << "     " << " - info: " << definedArg.help << endl;
            if (definedArg.mandatory) {
                cout << "     " << " - mandatory " << endl;
            } else {
                switch (definedArg.type) {
                    case STRING:
                        cout << "     " << " - default: " << definedArg.default_value.str_val << endl;
                        break;
                    case U_INT32:
                        cout << "     " << " - default: " << definedArg.default_value.uint32_val << endl;
                        break;
                    case U_INT16:
                        cout << "     " << " - default: " << definedArg.default_value.uint16_val << endl;
                        break;
                    case BOOL:
                        cout << "     " << " - default: " << definedArg.default_value.bool_val << endl;
                        break;
                }

            }
            cout << "\n\n";
        }
    }

private:
    bool populateDefaults() {
        for(auto const& value: definedArgs){
            string arg = value.first;
            if (resultsArgs.find(arg) == resultsArgs.end()) {
                if (value.second.mandatory) {
                    return false;
                }
                Arg resArg;
                resArg.type = value.second.type;
                resArg.value = value.second.default_value;
                resultsArgs[arg] = resArg;
            }

        }
        return true;
    }

    bool parseArgs(const vector<string> &argVec) {
        for (int i = 0; i < argVec.size(); i++){
            string arg = argVec[i];
            if (arg.find("--") != 0){
                return false;
            }
            arg = arg.erase(0,2);
            if(definedArgs.find(arg) == definedArgs.end()){
                return false;
            }

            if(resultsArgs.find(arg) != resultsArgs.end()){
                return false;
            }

            DefinedArg definedArg = definedArgs.find(arg)->second;
            switch (definedArg.type){
                case BOOL: {
                    Arg resArg;
                    resArg.type = BOOL;
                    resArg.value.bool_val = true;
                    resultsArgs[arg] = resArg;
                    break;
                }
                case U_INT16:
                case U_INT32:
                case STRING: {
                    i++;
                    if (i == (argVec.size())) {
                        return false;
                    }
                    string argVal = argVec[i];
                    Arg resArg;
                    switch (definedArg.type) {
                        case U_INT16:
                        case U_INT32:
                            try {
                                uint32_t num = (uint32_t)stoi(argVal);
                                switch (definedArg.type){
                                    case U_INT16:
                                        if(num < 0 || num > 0x0FFFF){
                                            return false;
                                        }
                                        resArg.type = U_INT16;
                                        resArg.value.uint16_val = (uint16_t)num;
                                        resultsArgs[arg] = resArg;
                                        break;
                                    case U_INT32:
                                        resArg.type = U_INT32;
                                        resArg.value.uint32_val = num;
                                        resultsArgs[arg] = resArg;
                                        break;
                                    default:
                                        return false;
                                }

                            }catch (exception& e){
                                return false;
                            }
                            break;
                        case STRING:
                            resArg.type = STRING;
                            resArg.value.str_val = argVec[i];
                            resultsArgs[arg] = resArg;
                            break;
                        default:
                            return false;
                    }
                }

            }
        }

        return true;
    }
};


#endif //TCP_SHMAFKA_ARGSPARSER_H
