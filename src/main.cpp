#include <iostream>
#include "Server/TcpServer.h"
#include "Client/Client.h"
#include "Logging/TSLogger.h"
#include "Chain/Chain.h"

class CliArgParser{

public:
    enum CliArgResType{UNDEFINED, SERVER, PRODUCES, CONSUMER};

    struct CliArgRes{
    public:
        CliArgResType type = UNDEFINED;
        uint16_t serverConsPort = 0;
        uint16_t serverProdPort = 0;
        string host = "";

    };


    bool parseArgs(int argc, char *argv[]){
        vector<string> cliArgsStrings;
        for (int i = 0; i < argc; i++){
            cliArgsStrings.emplace_back(argv[i]);
        }

        if( cliArgsStrings.size() < 2){
            cerr << "bad option number" << endl;
            return false;
        }

        if(cliArgsStrings[1] == "--server" || cliArgsStrings[1] == "-s"){
            cliArgRes.type = SERVER;
            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
                if(cliArgsStrings[i] == "--in-port" || cliArgsStrings[i] == "-i" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.serverProdPort = strToPort(cliArgsStrings[i]);
                        if (cliArgRes.serverProdPort == 0 ){
                            return false;
                        }
                    } else {
                        return false;
                    }
                }
                else if(cliArgsStrings[i] == "--out-port" || cliArgsStrings[i] == "-o" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.serverConsPort= strToPort(cliArgsStrings[i]);
                        if (cliArgRes.serverConsPort == 0 ){
                            return false;
                        }
                    } else {
                        return false;
                    }
                }

                else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.host = cliArgsStrings[i];
                    } else {
                        return false;
                    }
                }
            }
            return true;
        }else if(cliArgsStrings[1] == "--producer" || cliArgsStrings[1] == "-p"){
            cliArgRes.type = PRODUCES;
            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
                if(cliArgsStrings[i] == "--server-port" || cliArgsStrings[i] == "-s" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.serverProdPort = strToPort(cliArgsStrings[i]);
                        if (cliArgRes.serverProdPort == 0 ){
                            return false;
                        }
                    } else {
                        return false;
                    }
                }else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.host = cliArgsStrings[i];
                    } else {
                        return false;
                    }
                }
            }
            return true;
        }else if(cliArgsStrings[1] == "--consumer" || cliArgsStrings[1] == "-c"){
            cliArgRes.type = CONSUMER;
            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
                if(cliArgsStrings[i] == "--server-port" || cliArgsStrings[i] == "-s" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.serverConsPort = strToPort(cliArgsStrings[i]);
                        if (cliArgRes.serverConsPort== 0 ){
                            return false;
                        }
                    } else {
                        return false;
                    }
                }else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
                    if(i<cliArgsStrings.size()){
                        i++;
                        cliArgRes.host = cliArgsStrings[i];
                    } else {
                        return false;
                    }
                }
            }
            return true;
        } else{
            return false;
        }
    }

    CliArgRes &getCliArgRes(){
        return cliArgRes;
    }
private:
    uint16_t strToPort(const string &str){
        try {
            int p = stoi(str);
            if(p < 1024 || p > 0x0FFFF){
                return 0;
            }
            return (uint16_t)p;

        }catch (std::exception& e){
            return 0;
        }


    }

private:
    CliArgRes cliArgRes;


};

void runProducer(CliArgParser &argParser) {
    TSLogger::globalLogger->init("producer.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    boost::asio::io_service io_service;
    Client client(io_service, argParser.getCliArgRes().host, argParser.getCliArgRes().serverProdPort);
    thread pClient(boost::bind(&Client::run, &client));
    client.send(builder.buildPostRegistrationMsg("222222"));
    client.send(builder.buildPostRegistrationMsg("111111"));
    stringstream ss("");
    for (int i = 0 ; i < 2048; i++){
        ss << "hahahah sent ";
    }
    for (int i = 0 ; i < 2048; i++){
        ss << "ssasa sent ";
    }
    client.send(builder.buildPostPostChannelsResMsg("222222",ss.str()));
//    client.send(builder.buildPostDisconnectMsg());
//    for (int i = 0; i < 100 ; i++){
//        client.send(builder.buildPostPostChannelsResMsg("222222","hahahah sent"));
//        client.send(builder.buildPostPostChannelsResMsg("111111","hiuhihi sent"));
////        client.send(builder.buildPostDisconnectMsg());
//        usleep(10);
//    }

    for (int i = 0; i < 5 ; i++){

//        usleep(1000*5);
    }

    client.recieve()->print_hex_memory();
    pClient.join();
}

void runConsumer(CliArgParser &argParser) {
    TSLogger::globalLogger->init("consumer.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    boost::asio::io_service io_service;
    Client client(io_service, argParser.getCliArgRes().host, argParser.getCliArgRes().serverConsPort);
    thread pClient(boost::bind(&Client::run, &client));
    client.send(builder.buildListenRegistrationMsg("111111"));
    client.send(builder.buildListenRegistrationMsg("222222"));
    for (int i = 0; i < 1000000 ; i++) {
        auto msg = client.recieve();
        cout << i << endl;
        msg->print_hex_memory();
        BufferPool::bufferPool->release(msg);
    }
    pClient.join();
}

void runServer(CliArgParser &argParser) {
    TSLogger::globalLogger->init("server.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    Chain chain(argParser.getCliArgRes().serverConsPort, argParser.getCliArgRes().serverProdPort, &builder);
    chain.runThreads();
    chain.join();
}
int main(int argc, char *argv[]) {

    CliArgParser argParser;
    if (!argParser.parseArgs(argc, argv)){
        cerr << "bad parsing" << endl;
        return -1;
    }
    switch (argParser.getCliArgRes().type){
        case CliArgParser::SERVER: {
            runServer(argParser);
            break;
        }
        case CliArgParser::CONSUMER: {
            runConsumer(argParser);
            break;
        }
        case CliArgParser::PRODUCES: {
            runProducer(argParser);
            break;
        }
        default:
            cerr << "no type" << endl;
    }

    return 0;
}

