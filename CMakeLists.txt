cmake_minimum_required(VERSION 3.8)
project(tcp_shmafka)

set(CMAKE_CXX_STANDARD 11)




find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})


endif()

set(SOURCE_FILES src/main.cpp
        src/server.h
        src/server.cpp
        src/DefinedMessages.cpp
        src/DefinedMessages.h
        src/BufferPool.h
        src/BufferPool.cpp
        src/ConcurentQueue.h
        src/Configuration/Config.h
        src/Configuration/Config.cpp
        src/Configuration/GlobalConfig.cpp
        src/Configuration/GlobalConfig.h)

if(Boost_FOUND)

    include_directories(/usr/include/boost/asio/ip/)
    include_directories(external_libs/iniparser/src)
endif()
add_executable(tcp_shmafka ${SOURCE_FILES})
target_link_libraries(tcp_shmafka ${Boost_LIBRARIES})