#include <iostream>
#include "Server/TcpServer.h"
#include "Client/Client.h"
#include "Logging/TSLogger.h"
#include "Chain/Chain.h"
#include "ArgsParser.h"
#include "Queue/ConcurrentQueueSingleConsumer.h"
#include "Server/Epoll/EpollTcpServer.h"

//int run(int argc, char *const *argv);

//#include <ArgsParser.h>
//
//class CliArgParser{
//
//public:
//    enum CliArgResType{UNDEFINED, SERVER, PRODUCES, CONSUMER};
//
//    struct CliArgRes{
//    public:
//        CliArgResType type = UNDEFINED;
//        uint16_t serverConsPort = 0;
//        uint16_t serverProdPort = 0;
//        string host = "";
//
//    };
//
//
//    bool parseArgs(int argc, char *argv[]){
//        vector<string> cliArgsStrings;
//        for (int i = 0; i < argc; i++){
//            cliArgsStrings.emplace_back(argv[i]);
//        }
//
//        if( cliArgsStrings.size() < 2){
//            cerr << "bad option number" << endl;
//            return false;
//        }
//
//        if(cliArgsStrings[1] == "--server" || cliArgsStrings[1] == "-s"){
//            cliArgRes.type = SERVER;
//            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
//                if(cliArgsStrings[i] == "--in-port" || cliArgsStrings[i] == "-i" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.serverProdPort = strToPort(cliArgsStrings[i]);
//                        if (cliArgRes.serverProdPort == 0 ){
//                            return false;
//                        }
//                    } else {
//                        return false;
//                    }
//                }
//                else if(cliArgsStrings[i] == "--out-port" || cliArgsStrings[i] == "-o" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.serverConsPort= strToPort(cliArgsStrings[i]);
//                        if (cliArgRes.serverConsPort == 0 ){
//                            return false;
//                        }
//                    } else {
//                        return false;
//                    }
//                }
//
//                else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.host = cliArgsStrings[i];
//                    } else {
//                        return false;
//                    }
//                }
//            }
//            return true;
//        }else if(cliArgsStrings[1] == "--producer" || cliArgsStrings[1] == "-p"){
//            cliArgRes.type = PRODUCES;
//            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
//                if(cliArgsStrings[i] == "--server-port" || cliArgsStrings[i] == "-s" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.serverProdPort = strToPort(cliArgsStrings[i]);
//                        if (cliArgRes.serverProdPort == 0 ){
//                            return false;
//                        }
//                    } else {
//                        return false;
//                    }
//                }else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.host = cliArgsStrings[i];
//                    } else {
//                        return false;
//                    }
//                }
//            }
//            return true;
//        }else if(cliArgsStrings[1] == "--consumer" || cliArgsStrings[1] == "-c"){
//            cliArgRes.type = CONSUMER;
//            for (int i = 2 ; i < cliArgsStrings.size() ; i++){
//                if(cliArgsStrings[i] == "--server-port" || cliArgsStrings[i] == "-s" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.serverConsPort = strToPort(cliArgsStrings[i]);
//                        if (cliArgRes.serverConsPort== 0 ){
//                            return false;
//                        }
//                    } else {
//                        return false;
//                    }
//                }else if(cliArgsStrings[i] == "--host" || cliArgsStrings[i] == "-h" ){
//                    if(i<cliArgsStrings.size()){
//                        i++;
//                        cliArgRes.host = cliArgsStrings[i];
//                    } else {
//                        return false;
//                    }
//                }
//            }
//            return true;
//        } else{
//            return false;
//        }
//    }
//
//    CliArgRes &getCliArgRes(){
//        return cliArgRes;
//    }
//private:
//    uint16_t strToPort(const string &str){
//        try {
//            int p = stoi(str);
//            if(p < 1024 || p > 0x0FFFF){
//                return 0;
//            }
//            return (uint16_t)p;
//
//        }catch (std::exception& e){
//            return 0;
//        }
//
//
//    }
//
//private:
//    CliArgRes cliArgRes;
//
//
//};
//



void runProducer(string &serverHost,uint16_t producerPort) {
    TSLogger::globalLogger->init("producer.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    boost::asio::io_service io_service;
    Client client(io_service, serverHost, producerPort, SLEEP);
    thread pClient(boost::bind(&Client::run, &client));
    usleep(1000000000);
//    exit(0);
    client.send(builder.buildPostRegistrationMsg("222222"));
    client.send(builder.buildPostRegistrationMsg("111111"));
    int z = 3500000;
    stringstream ss("");
    for (int i = 0; i < 20; i++) {
        ss << "hahahah sent ";
    }
    for (int i = 0; i < 20; i++) {
        ss << "ssasa sent ";
    }

    auto buff = builder.buildPostPostChannelsResMsg("222222", ss.str());
    while (z > 0 ) {
        z--;
//        cout << z <<endl;

        if (buff == nullptr){
//            LOG_ERROR("no buffers client");
            usleep(10);
        } else{
            client.send(buff);
        }

//        cout << "end\n";
        for (int i = 0; i < 5; i++) {

//        usleep(1000*5);
        }

//        client.recieve()->print_hex_memory();
    }
    pClient.join();
}
//
void runConsumer(string &serverHost,uint16_t consumerPort) {
    TSLogger::globalLogger->init("consumer.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    boost::asio::io_service io_service;
    Client client(io_service, serverHost, consumerPort, SLEEP);
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

void runServer(uint16_t consumerPort, uint16_t producerPort) {
    TSLogger::globalLogger->init("server.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    Chain chain(consumerPort, producerPort, &builder, 1024, SLEEP);
    chain.runThreads();
    chain.join();
}

int run(int argc, char *argv[]) {
    ArgsParser argParser;
    argParser.addUint16ArgWithDefault("prod-port", "tcp port for producer stream",8282);
    argParser.addUint16ArgWithDefault("cons-port", "tcp port for consumer stream",8281);
    argParser.addStrArgWithDefault("server-host", "server host address", "127.0.0.1");
    argParser.addStrArg("type", "consumer/producer/server");
    if(!argParser.generateArgs(argc-1,&argv[1])){
        argParser.printUsage();
        return -1;
    }

    string type = argParser.getResults()["type"].value.str_val;
    string serverHost = argParser.getResults()["server-host"].value.str_val;
    uint16_t producerPort = argParser.getResults()["prod-port"].value.uint16_val;
    uint16_t consumerPort = argParser.getResults()["cons-port"].value.uint16_val;

    if(type == "server"){
        runServer(consumerPort, producerPort);
    } else if (type == "producer"){
        runProducer(serverHost, producerPort);
    } else if (type == "consumer"){
        runConsumer(serverHost, consumerPort);
    }
    return 0;
}
//int z = 9;
//class S{
//public:
//    explicit S(const string &g) : g(g) {
//        kaka = new char[50];
//        strcpy(kaka,(char *)g.c_str());
//    }
//    virtual ~S() {
//        cout << this << "   kkkk  "  << g << "  "<< kaka << endl;
//    }
//    S( S &obj){
//        kaka = obj.kaka;
//        cout << "cc22cc" << endl;
//    }
//
//    S(S&& other){
//        z=6;
//    }
//    S& operator=(S&& other){
//        z=2;
//    }
//
//
//
//    char *kaka{};
//    string g;
//};
//
//
//
//void func( S &&s){
//    cout << "sss" << s.kaka;
//}
int main(int argc, char *argv[]) {
    TSLogger::globalLogger->init("consumer.log", true, TSLogger::TRACE);
    EpollTcpServer tcpServer;
    tcpServer.test();



//    shared_ptr<S> ptr2;
//    S s("xxx");
//
//    IConcurrentQueue<shared_ptr<S>> * queue1 = new ConcurrentQueueSingleConsumer<shared_ptr<S>>(10,SLEEP);
////    S &&k = ;
//    {
//        vector<S> vector1;
//        char *data = (char *) "ssssss";
//
//        vector1.push_back(std::move(s));
//
//        shared_ptr<S> ptr = make_shared<S>(s);
////    shared_ptr<S> ptr2 = std::move;
//        func(std::move(s));
////        S &&k = std::move(s);
//    }
//    cout << z << endl;
////    queue1->push(make_shared<S>(s));
//
//
//
//
////
////    queue1->push(ptr);
////    char * z = * (queue1->pop().get());
////    cout << z << endl;
////    auto zz = (queue1->try_pop());
////    if(zz == nullptr){
////        cout << "ddd" ;
////    }
////    cout << (uint64_t)z << endl;
   return run(argc, argv);

}



