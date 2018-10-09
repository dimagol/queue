#include <iostream>
#include "Logging/TSLogger.h"
#include "Chain/Chain.h"
#include "ArgsParser.h"
#include "Server/Epoll/SimpleMsgListBuilder.h"
#include "Test/Client.h"


void runProducer(string &serverHost,uint16_t producerPort) {


    tcp_client c;


    //connect to host
    c.conn(serverHost , producerPort);

    //send some data
        string s = "";
    for (int i =0; i < 1500 ; i++){
        s += "s";
    }
    SimpleMsgListBuilder simpleMsgListBuilder;
    const char * data = s.c_str();
    auto  l = simpleMsgListBuilder.buildFromBuff((uint8_t *)data, strlen(data));
//    l->get_head()->print_hex_memory();
//    l->get_head()->nextBuffer->print_hex_memory();

    uint8_t * toSend = new uint8_t(2048);
    memcpy(toSend , l->get_head()->msg_complete_buff, l->get_head()->get_msg_len());
    memcpy(toSend + l->get_head()->get_msg_len(), l->get_head()->nextBuffer->msg_complete_buff,l->get_head()->nextBuffer->get_msg_len());
    c.send_data(toSend,l->get_head()->get_msg_len() + l->get_head()->nextBuffer->get_msg_len());
//    c.send_data(l->get_head()->msg_complete_buff,l->get_head()->get_msg_len());
//    c.send_data(l->get_head()->nextBuffer->msg_complete_buff,l->get_head()->nextBuffer->get_msg_len());

    //receive and echo reply
    cout<<"----------------------------\n\n";
    c.receive(1024);
    cout<<"\n\n----------------------------\n\n";


    //receive and echo reply
    cout<<"----------------------------\n\n";
    c.receive(1024);
    cout<<"\n\n----------------------------\n\n";


    //receive and echo reply
    cout<<"----------------------------\n\n";
    c.receive(1024);
    cout<<"\n\n----------------------------\n\n";



    //receive and echo reply
    cout<<"----------------------------\n\n";
    c.receive(1024);
    cout<<"\n\n----------------------------\n\n";


    //receive and echo reply
    cout<<"----------------------------\n\n";
    c.receive(1024);
    cout<<"\n\n----------------------------\n\n";


//    TSLogger::globalLogger->init("producer.log", true, TSLogger::TRACE);
//    MsgBuilder builder(BufferPool::bufferPool);
//    boost::asio::io_service io_service;
//    Client client(io_service, serverHost, producerPort, SLEEP);
//    thread pClient(boost::bind(&Client::run, &client));
////    usleep(1000000000);
////    exit(0);
//    client.send(builder.buildPostRegistrationMsg("222222"));
//    client.send(builder.buildPostRegistrationMsg("111111"));
//    int z = 3500000;
//    stringstream ss("");
//    for (int i = 0; i < 20; i++) {
//        ss << "hahahah sent ";
//    }
//    for (int i = 0; i < 20; i++) {
//        ss << "ssasa sent ";
//    }
//
//    auto buff = builder.buildPostPostChannelsResMsg("222222", ss.str());
//    while (z > 0 ) {
//        z--;
////        cout << z <<endl;
//
//        if (buff == nullptr){
////            LOG_ERROR("no buffers client");
//            usleep(10);
//        } else{
//            client.send(buff);
//        }
//
////        cout << "end\n";
//        for (int i = 0; i < 5; i++) {
//
////        usleep(1000*5);
//        }
//
////        client.recieve()->print_hex_memory();
//    }
//    pClient.join();
}
//
void runConsumer(string &serverHost,uint16_t consumerPort) {
//    TSLogger::globalLogger->init("consumer.log", true, TSLogger::TRACE);
//    MsgBuilder builder(BufferPool::bufferPool);
//    boost::asio::io_service io_service;
//    Client client(io_service, serverHost, consumerPort, SLEEP);
//    thread pClient(boost::bind(&Client::run, &client));
//    client.send(builder.buildListenRegistrationMsg("111111"));
//    client.send(builder.buildListenRegistrationMsg("222222"));
//    for (int i = 0; i < 1000000 ; i++) {
//        auto msg = client.recieve();
//        cout << i << endl;
//        msg->print_hex_memory();
//        BufferPool::bufferPool->releaseAllChain(msg);
//    }
//    pClient.join();
}

void runServer(uint16_t consumerPort, uint16_t producerPort, string &lAdrr) {


//    string s = "";
//    for (int i =0; i < 1013 ; i++){
//        s += "s";
//    }
//    const char * data = s.c_str();
//    auto  l = simpleMsgListBuilder.buildFromBuff((uint8_t *)data, strlen(data));
//    l->get_head()->print_hex_memory();
//    l->get_head()->nextBuffer->print_hex_memory();
    TSLogger::globalLogger->init("server.log", true, TSLogger::TRACE);
    MsgBuilder builder(BufferPool::bufferPool);
    Chain chain(consumerPort, producerPort, &builder, 1024, SLEEP, lAdrr);
    chain.runThreads();
    auto data = chain.getProducerTcpServer().recieve();
    data->getBufferList()->get_head()->print_hex_memory();
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
        runServer(consumerPort, producerPort, serverHost);
    } else if (type == "producer"){
        runProducer(serverHost, producerPort);
    } else if (type == "consumer"){
        runConsumer(serverHost, consumerPort);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    TSLogger::globalLogger->init("consumer.log", true, TSLogger::TRACE);
    return run(argc, argv);
}



