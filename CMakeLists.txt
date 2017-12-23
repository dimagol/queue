cmake_minimum_required(VERSION 3.8)
project(tcp_shmafka)
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD 11)




find_package(Boost 1.58.0 COMPONENTS filesystem system REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})


endif()

set(SOURCE_FILES src/main.cpp
        src/Server/TcpServer.h
        src/Server/TcpServer.cpp
        src/DefinedMessages.cpp
        src/DefinedMessages.h
        src/Buff/BufferPool.h
        src/Buff/BufferPool.cpp
        src/Queue/ConcurentQueue.h
        src/Configuration/dictionary.h
        src/Configuration/dictionary.cpp
        src/Configuration/iniparser.h
        src/Configuration/iniparser.cpp
        src/Configuration/Config.h
        src/Configuration/Config.cpp
        src/Configuration/GlobalConfig.cpp
        src/Configuration/GlobalConfig.h
        src/Server/TcpServerConnection.cpp
        src/Server/TcpServerConnection.h
        src/Client/Client.cpp
        src/Client/Client.h
        src/Logging/TSLogger.h src/Processor/MsgToEventProcessor.cpp src/Processor/MsgToEventProcessor.h src/Processor/ProceededEvent.cpp src/Processor/ProceededEvent.h src/Server/TcpServerIncomeMessage.h src/Msg/MsgType.h src/Msg/MsgConsts.h src/ChanelDb/ChannelDb.cpp src/ChanelDb/ChannelDb.h src/ChanelDb/Channel.cpp src/ChanelDb/Channel.h src/Server/TcpServerOutcomeMessage.h src/Threads/ServerThread.cpp src/Threads/ServerThread.h src/Worker/Worker.cpp src/Worker/Worker.h src/Buff/SocketProtoBuffer.h src/Threads/WorkerThread.cpp src/Threads/WorkerThread.h src/Chain/Chain.cpp src/Chain/Chain.h src/Msg/MsgBuilder.cpp src/Msg/MsgBuilder.h src/Logging/TSLogger.cpp src/Buff/stacktrace.h)

if(Boost_FOUND)
    include_directories(
            /usr/include/boost/asio/ip/
            /usr/include/boost/log/
            /usr/include/boost/date_time
    )
endif()
#here
set(CMAKE_CXX_FLAGS " -pthread")
add_executable(tcp_shmafka ${SOURCE_FILES})
target_link_libraries(tcp_shmafka ${Boost_LIBRARIES})
