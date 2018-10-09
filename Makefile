# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/dima/clion-2017.2.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/dima/clion-2017.2.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dima/CLionProjects/tcp_shmafka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dima/CLionProjects/tcp_shmafka

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/dima/clion-2017.2.1/bin/cmake/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/dima/clion-2017.2.1/bin/cmake/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dima/CLionProjects/tcp_shmafka/CMakeFiles /home/dima/CLionProjects/tcp_shmafka/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dima/CLionProjects/tcp_shmafka/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named tcp_shmafka

# Build rule for target.
tcp_shmafka: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 tcp_shmafka
.PHONY : tcp_shmafka

# fast build rule for target.
tcp_shmafka/fast:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/build
.PHONY : tcp_shmafka/fast

src/ArgsParser.o: src/ArgsParser.cpp.o

.PHONY : src/ArgsParser.o

# target to build an object file
src/ArgsParser.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ArgsParser.cpp.o
.PHONY : src/ArgsParser.cpp.o

src/ArgsParser.i: src/ArgsParser.cpp.i

.PHONY : src/ArgsParser.i

# target to preprocess a source file
src/ArgsParser.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ArgsParser.cpp.i
.PHONY : src/ArgsParser.cpp.i

src/ArgsParser.s: src/ArgsParser.cpp.s

.PHONY : src/ArgsParser.s

# target to generate assembly for a file
src/ArgsParser.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ArgsParser.cpp.s
.PHONY : src/ArgsParser.cpp.s

src/Buff/BufferPool.o: src/Buff/BufferPool.cpp.o

.PHONY : src/Buff/BufferPool.o

# target to build an object file
src/Buff/BufferPool.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Buff/BufferPool.cpp.o
.PHONY : src/Buff/BufferPool.cpp.o

src/Buff/BufferPool.i: src/Buff/BufferPool.cpp.i

.PHONY : src/Buff/BufferPool.i

# target to preprocess a source file
src/Buff/BufferPool.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Buff/BufferPool.cpp.i
.PHONY : src/Buff/BufferPool.cpp.i

src/Buff/BufferPool.s: src/Buff/BufferPool.cpp.s

.PHONY : src/Buff/BufferPool.s

# target to generate assembly for a file
src/Buff/BufferPool.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Buff/BufferPool.cpp.s
.PHONY : src/Buff/BufferPool.cpp.s

src/Chain/Chain.o: src/Chain/Chain.cpp.o

.PHONY : src/Chain/Chain.o

# target to build an object file
src/Chain/Chain.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Chain/Chain.cpp.o
.PHONY : src/Chain/Chain.cpp.o

src/Chain/Chain.i: src/Chain/Chain.cpp.i

.PHONY : src/Chain/Chain.i

# target to preprocess a source file
src/Chain/Chain.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Chain/Chain.cpp.i
.PHONY : src/Chain/Chain.cpp.i

src/Chain/Chain.s: src/Chain/Chain.cpp.s

.PHONY : src/Chain/Chain.s

# target to generate assembly for a file
src/Chain/Chain.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Chain/Chain.cpp.s
.PHONY : src/Chain/Chain.cpp.s

src/ChanelDb/Channel.o: src/ChanelDb/Channel.cpp.o

.PHONY : src/ChanelDb/Channel.o

# target to build an object file
src/ChanelDb/Channel.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/Channel.cpp.o
.PHONY : src/ChanelDb/Channel.cpp.o

src/ChanelDb/Channel.i: src/ChanelDb/Channel.cpp.i

.PHONY : src/ChanelDb/Channel.i

# target to preprocess a source file
src/ChanelDb/Channel.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/Channel.cpp.i
.PHONY : src/ChanelDb/Channel.cpp.i

src/ChanelDb/Channel.s: src/ChanelDb/Channel.cpp.s

.PHONY : src/ChanelDb/Channel.s

# target to generate assembly for a file
src/ChanelDb/Channel.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/Channel.cpp.s
.PHONY : src/ChanelDb/Channel.cpp.s

src/ChanelDb/ChannelDb.o: src/ChanelDb/ChannelDb.cpp.o

.PHONY : src/ChanelDb/ChannelDb.o

# target to build an object file
src/ChanelDb/ChannelDb.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/ChannelDb.cpp.o
.PHONY : src/ChanelDb/ChannelDb.cpp.o

src/ChanelDb/ChannelDb.i: src/ChanelDb/ChannelDb.cpp.i

.PHONY : src/ChanelDb/ChannelDb.i

# target to preprocess a source file
src/ChanelDb/ChannelDb.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/ChannelDb.cpp.i
.PHONY : src/ChanelDb/ChannelDb.cpp.i

src/ChanelDb/ChannelDb.s: src/ChanelDb/ChannelDb.cpp.s

.PHONY : src/ChanelDb/ChannelDb.s

# target to generate assembly for a file
src/ChanelDb/ChannelDb.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/ChanelDb/ChannelDb.cpp.s
.PHONY : src/ChanelDb/ChannelDb.cpp.s

src/Configuration/Config.o: src/Configuration/Config.cpp.o

.PHONY : src/Configuration/Config.o

# target to build an object file
src/Configuration/Config.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/Config.cpp.o
.PHONY : src/Configuration/Config.cpp.o

src/Configuration/Config.i: src/Configuration/Config.cpp.i

.PHONY : src/Configuration/Config.i

# target to preprocess a source file
src/Configuration/Config.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/Config.cpp.i
.PHONY : src/Configuration/Config.cpp.i

src/Configuration/Config.s: src/Configuration/Config.cpp.s

.PHONY : src/Configuration/Config.s

# target to generate assembly for a file
src/Configuration/Config.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/Config.cpp.s
.PHONY : src/Configuration/Config.cpp.s

src/Configuration/GlobalConfig.o: src/Configuration/GlobalConfig.cpp.o

.PHONY : src/Configuration/GlobalConfig.o

# target to build an object file
src/Configuration/GlobalConfig.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/GlobalConfig.cpp.o
.PHONY : src/Configuration/GlobalConfig.cpp.o

src/Configuration/GlobalConfig.i: src/Configuration/GlobalConfig.cpp.i

.PHONY : src/Configuration/GlobalConfig.i

# target to preprocess a source file
src/Configuration/GlobalConfig.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/GlobalConfig.cpp.i
.PHONY : src/Configuration/GlobalConfig.cpp.i

src/Configuration/GlobalConfig.s: src/Configuration/GlobalConfig.cpp.s

.PHONY : src/Configuration/GlobalConfig.s

# target to generate assembly for a file
src/Configuration/GlobalConfig.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/GlobalConfig.cpp.s
.PHONY : src/Configuration/GlobalConfig.cpp.s

src/Configuration/dictionary.o: src/Configuration/dictionary.cpp.o

.PHONY : src/Configuration/dictionary.o

# target to build an object file
src/Configuration/dictionary.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/dictionary.cpp.o
.PHONY : src/Configuration/dictionary.cpp.o

src/Configuration/dictionary.i: src/Configuration/dictionary.cpp.i

.PHONY : src/Configuration/dictionary.i

# target to preprocess a source file
src/Configuration/dictionary.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/dictionary.cpp.i
.PHONY : src/Configuration/dictionary.cpp.i

src/Configuration/dictionary.s: src/Configuration/dictionary.cpp.s

.PHONY : src/Configuration/dictionary.s

# target to generate assembly for a file
src/Configuration/dictionary.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/dictionary.cpp.s
.PHONY : src/Configuration/dictionary.cpp.s

src/Configuration/iniparser.o: src/Configuration/iniparser.cpp.o

.PHONY : src/Configuration/iniparser.o

# target to build an object file
src/Configuration/iniparser.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/iniparser.cpp.o
.PHONY : src/Configuration/iniparser.cpp.o

src/Configuration/iniparser.i: src/Configuration/iniparser.cpp.i

.PHONY : src/Configuration/iniparser.i

# target to preprocess a source file
src/Configuration/iniparser.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/iniparser.cpp.i
.PHONY : src/Configuration/iniparser.cpp.i

src/Configuration/iniparser.s: src/Configuration/iniparser.cpp.s

.PHONY : src/Configuration/iniparser.s

# target to generate assembly for a file
src/Configuration/iniparser.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Configuration/iniparser.cpp.s
.PHONY : src/Configuration/iniparser.cpp.s

src/Logging/TSLogger.o: src/Logging/TSLogger.cpp.o

.PHONY : src/Logging/TSLogger.o

# target to build an object file
src/Logging/TSLogger.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Logging/TSLogger.cpp.o
.PHONY : src/Logging/TSLogger.cpp.o

src/Logging/TSLogger.i: src/Logging/TSLogger.cpp.i

.PHONY : src/Logging/TSLogger.i

# target to preprocess a source file
src/Logging/TSLogger.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Logging/TSLogger.cpp.i
.PHONY : src/Logging/TSLogger.cpp.i

src/Logging/TSLogger.s: src/Logging/TSLogger.cpp.s

.PHONY : src/Logging/TSLogger.s

# target to generate assembly for a file
src/Logging/TSLogger.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Logging/TSLogger.cpp.s
.PHONY : src/Logging/TSLogger.cpp.s

src/Msg/MsgBuilder.o: src/Msg/MsgBuilder.cpp.o

.PHONY : src/Msg/MsgBuilder.o

# target to build an object file
src/Msg/MsgBuilder.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Msg/MsgBuilder.cpp.o
.PHONY : src/Msg/MsgBuilder.cpp.o

src/Msg/MsgBuilder.i: src/Msg/MsgBuilder.cpp.i

.PHONY : src/Msg/MsgBuilder.i

# target to preprocess a source file
src/Msg/MsgBuilder.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Msg/MsgBuilder.cpp.i
.PHONY : src/Msg/MsgBuilder.cpp.i

src/Msg/MsgBuilder.s: src/Msg/MsgBuilder.cpp.s

.PHONY : src/Msg/MsgBuilder.s

# target to generate assembly for a file
src/Msg/MsgBuilder.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Msg/MsgBuilder.cpp.s
.PHONY : src/Msg/MsgBuilder.cpp.s

src/Processor/MsgToEventProcessor.o: src/Processor/MsgToEventProcessor.cpp.o

.PHONY : src/Processor/MsgToEventProcessor.o

# target to build an object file
src/Processor/MsgToEventProcessor.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/MsgToEventProcessor.cpp.o
.PHONY : src/Processor/MsgToEventProcessor.cpp.o

src/Processor/MsgToEventProcessor.i: src/Processor/MsgToEventProcessor.cpp.i

.PHONY : src/Processor/MsgToEventProcessor.i

# target to preprocess a source file
src/Processor/MsgToEventProcessor.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/MsgToEventProcessor.cpp.i
.PHONY : src/Processor/MsgToEventProcessor.cpp.i

src/Processor/MsgToEventProcessor.s: src/Processor/MsgToEventProcessor.cpp.s

.PHONY : src/Processor/MsgToEventProcessor.s

# target to generate assembly for a file
src/Processor/MsgToEventProcessor.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/MsgToEventProcessor.cpp.s
.PHONY : src/Processor/MsgToEventProcessor.cpp.s

src/Processor/ProceededEvent.o: src/Processor/ProceededEvent.cpp.o

.PHONY : src/Processor/ProceededEvent.o

# target to build an object file
src/Processor/ProceededEvent.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/ProceededEvent.cpp.o
.PHONY : src/Processor/ProceededEvent.cpp.o

src/Processor/ProceededEvent.i: src/Processor/ProceededEvent.cpp.i

.PHONY : src/Processor/ProceededEvent.i

# target to preprocess a source file
src/Processor/ProceededEvent.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/ProceededEvent.cpp.i
.PHONY : src/Processor/ProceededEvent.cpp.i

src/Processor/ProceededEvent.s: src/Processor/ProceededEvent.cpp.s

.PHONY : src/Processor/ProceededEvent.s

# target to generate assembly for a file
src/Processor/ProceededEvent.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Processor/ProceededEvent.cpp.s
.PHONY : src/Processor/ProceededEvent.cpp.s

src/Queue/AsyncLimitedQueue.o: src/Queue/AsyncLimitedQueue.cpp.o

.PHONY : src/Queue/AsyncLimitedQueue.o

# target to build an object file
src/Queue/AsyncLimitedQueue.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Queue/AsyncLimitedQueue.cpp.o
.PHONY : src/Queue/AsyncLimitedQueue.cpp.o

src/Queue/AsyncLimitedQueue.i: src/Queue/AsyncLimitedQueue.cpp.i

.PHONY : src/Queue/AsyncLimitedQueue.i

# target to preprocess a source file
src/Queue/AsyncLimitedQueue.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Queue/AsyncLimitedQueue.cpp.i
.PHONY : src/Queue/AsyncLimitedQueue.cpp.i

src/Queue/AsyncLimitedQueue.s: src/Queue/AsyncLimitedQueue.cpp.s

.PHONY : src/Queue/AsyncLimitedQueue.s

# target to generate assembly for a file
src/Queue/AsyncLimitedQueue.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Queue/AsyncLimitedQueue.cpp.s
.PHONY : src/Queue/AsyncLimitedQueue.cpp.s

src/Server/Epoll/EpollTcpServer.o: src/Server/Epoll/EpollTcpServer.cpp.o

.PHONY : src/Server/Epoll/EpollTcpServer.o

# target to build an object file
src/Server/Epoll/EpollTcpServer.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/EpollTcpServer.cpp.o
.PHONY : src/Server/Epoll/EpollTcpServer.cpp.o

src/Server/Epoll/EpollTcpServer.i: src/Server/Epoll/EpollTcpServer.cpp.i

.PHONY : src/Server/Epoll/EpollTcpServer.i

# target to preprocess a source file
src/Server/Epoll/EpollTcpServer.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/EpollTcpServer.cpp.i
.PHONY : src/Server/Epoll/EpollTcpServer.cpp.i

src/Server/Epoll/EpollTcpServer.s: src/Server/Epoll/EpollTcpServer.cpp.s

.PHONY : src/Server/Epoll/EpollTcpServer.s

# target to generate assembly for a file
src/Server/Epoll/EpollTcpServer.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/EpollTcpServer.cpp.s
.PHONY : src/Server/Epoll/EpollTcpServer.cpp.s

src/Server/Epoll/MsgToClients.o: src/Server/Epoll/MsgToClients.cpp.o

.PHONY : src/Server/Epoll/MsgToClients.o

# target to build an object file
src/Server/Epoll/MsgToClients.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/MsgToClients.cpp.o
.PHONY : src/Server/Epoll/MsgToClients.cpp.o

src/Server/Epoll/MsgToClients.i: src/Server/Epoll/MsgToClients.cpp.i

.PHONY : src/Server/Epoll/MsgToClients.i

# target to preprocess a source file
src/Server/Epoll/MsgToClients.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/MsgToClients.cpp.i
.PHONY : src/Server/Epoll/MsgToClients.cpp.i

src/Server/Epoll/MsgToClients.s: src/Server/Epoll/MsgToClients.cpp.s

.PHONY : src/Server/Epoll/MsgToClients.s

# target to generate assembly for a file
src/Server/Epoll/MsgToClients.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/MsgToClients.cpp.s
.PHONY : src/Server/Epoll/MsgToClients.cpp.s

src/Server/Epoll/SimpleMsgListBuilder.o: src/Server/Epoll/SimpleMsgListBuilder.cpp.o

.PHONY : src/Server/Epoll/SimpleMsgListBuilder.o

# target to build an object file
src/Server/Epoll/SimpleMsgListBuilder.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/SimpleMsgListBuilder.cpp.o
.PHONY : src/Server/Epoll/SimpleMsgListBuilder.cpp.o

src/Server/Epoll/SimpleMsgListBuilder.i: src/Server/Epoll/SimpleMsgListBuilder.cpp.i

.PHONY : src/Server/Epoll/SimpleMsgListBuilder.i

# target to preprocess a source file
src/Server/Epoll/SimpleMsgListBuilder.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/SimpleMsgListBuilder.cpp.i
.PHONY : src/Server/Epoll/SimpleMsgListBuilder.cpp.i

src/Server/Epoll/SimpleMsgListBuilder.s: src/Server/Epoll/SimpleMsgListBuilder.cpp.s

.PHONY : src/Server/Epoll/SimpleMsgListBuilder.s

# target to generate assembly for a file
src/Server/Epoll/SimpleMsgListBuilder.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Server/Epoll/SimpleMsgListBuilder.cpp.s
.PHONY : src/Server/Epoll/SimpleMsgListBuilder.cpp.s

src/Threads/ServerThread.o: src/Threads/ServerThread.cpp.o

.PHONY : src/Threads/ServerThread.o

# target to build an object file
src/Threads/ServerThread.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/ServerThread.cpp.o
.PHONY : src/Threads/ServerThread.cpp.o

src/Threads/ServerThread.i: src/Threads/ServerThread.cpp.i

.PHONY : src/Threads/ServerThread.i

# target to preprocess a source file
src/Threads/ServerThread.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/ServerThread.cpp.i
.PHONY : src/Threads/ServerThread.cpp.i

src/Threads/ServerThread.s: src/Threads/ServerThread.cpp.s

.PHONY : src/Threads/ServerThread.s

# target to generate assembly for a file
src/Threads/ServerThread.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/ServerThread.cpp.s
.PHONY : src/Threads/ServerThread.cpp.s

src/Threads/WorkerThread.o: src/Threads/WorkerThread.cpp.o

.PHONY : src/Threads/WorkerThread.o

# target to build an object file
src/Threads/WorkerThread.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/WorkerThread.cpp.o
.PHONY : src/Threads/WorkerThread.cpp.o

src/Threads/WorkerThread.i: src/Threads/WorkerThread.cpp.i

.PHONY : src/Threads/WorkerThread.i

# target to preprocess a source file
src/Threads/WorkerThread.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/WorkerThread.cpp.i
.PHONY : src/Threads/WorkerThread.cpp.i

src/Threads/WorkerThread.s: src/Threads/WorkerThread.cpp.s

.PHONY : src/Threads/WorkerThread.s

# target to generate assembly for a file
src/Threads/WorkerThread.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Threads/WorkerThread.cpp.s
.PHONY : src/Threads/WorkerThread.cpp.s

src/WaitingStrategy.o: src/WaitingStrategy.cpp.o

.PHONY : src/WaitingStrategy.o

# target to build an object file
src/WaitingStrategy.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/WaitingStrategy.cpp.o
.PHONY : src/WaitingStrategy.cpp.o

src/WaitingStrategy.i: src/WaitingStrategy.cpp.i

.PHONY : src/WaitingStrategy.i

# target to preprocess a source file
src/WaitingStrategy.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/WaitingStrategy.cpp.i
.PHONY : src/WaitingStrategy.cpp.i

src/WaitingStrategy.s: src/WaitingStrategy.cpp.s

.PHONY : src/WaitingStrategy.s

# target to generate assembly for a file
src/WaitingStrategy.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/WaitingStrategy.cpp.s
.PHONY : src/WaitingStrategy.cpp.s

src/Worker/Worker.o: src/Worker/Worker.cpp.o

.PHONY : src/Worker/Worker.o

# target to build an object file
src/Worker/Worker.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Worker/Worker.cpp.o
.PHONY : src/Worker/Worker.cpp.o

src/Worker/Worker.i: src/Worker/Worker.cpp.i

.PHONY : src/Worker/Worker.i

# target to preprocess a source file
src/Worker/Worker.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Worker/Worker.cpp.i
.PHONY : src/Worker/Worker.cpp.i

src/Worker/Worker.s: src/Worker/Worker.cpp.s

.PHONY : src/Worker/Worker.s

# target to generate assembly for a file
src/Worker/Worker.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/Worker/Worker.cpp.s
.PHONY : src/Worker/Worker.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/tcp_shmafka.dir/build.make CMakeFiles/tcp_shmafka.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... tcp_shmafka"
	@echo "... src/ArgsParser.o"
	@echo "... src/ArgsParser.i"
	@echo "... src/ArgsParser.s"
	@echo "... src/Buff/BufferPool.o"
	@echo "... src/Buff/BufferPool.i"
	@echo "... src/Buff/BufferPool.s"
	@echo "... src/Chain/Chain.o"
	@echo "... src/Chain/Chain.i"
	@echo "... src/Chain/Chain.s"
	@echo "... src/ChanelDb/Channel.o"
	@echo "... src/ChanelDb/Channel.i"
	@echo "... src/ChanelDb/Channel.s"
	@echo "... src/ChanelDb/ChannelDb.o"
	@echo "... src/ChanelDb/ChannelDb.i"
	@echo "... src/ChanelDb/ChannelDb.s"
	@echo "... src/Configuration/Config.o"
	@echo "... src/Configuration/Config.i"
	@echo "... src/Configuration/Config.s"
	@echo "... src/Configuration/GlobalConfig.o"
	@echo "... src/Configuration/GlobalConfig.i"
	@echo "... src/Configuration/GlobalConfig.s"
	@echo "... src/Configuration/dictionary.o"
	@echo "... src/Configuration/dictionary.i"
	@echo "... src/Configuration/dictionary.s"
	@echo "... src/Configuration/iniparser.o"
	@echo "... src/Configuration/iniparser.i"
	@echo "... src/Configuration/iniparser.s"
	@echo "... src/Logging/TSLogger.o"
	@echo "... src/Logging/TSLogger.i"
	@echo "... src/Logging/TSLogger.s"
	@echo "... src/Msg/MsgBuilder.o"
	@echo "... src/Msg/MsgBuilder.i"
	@echo "... src/Msg/MsgBuilder.s"
	@echo "... src/Processor/MsgToEventProcessor.o"
	@echo "... src/Processor/MsgToEventProcessor.i"
	@echo "... src/Processor/MsgToEventProcessor.s"
	@echo "... src/Processor/ProceededEvent.o"
	@echo "... src/Processor/ProceededEvent.i"
	@echo "... src/Processor/ProceededEvent.s"
	@echo "... src/Queue/AsyncLimitedQueue.o"
	@echo "... src/Queue/AsyncLimitedQueue.i"
	@echo "... src/Queue/AsyncLimitedQueue.s"
	@echo "... src/Server/Epoll/EpollTcpServer.o"
	@echo "... src/Server/Epoll/EpollTcpServer.i"
	@echo "... src/Server/Epoll/EpollTcpServer.s"
	@echo "... src/Server/Epoll/MsgToClients.o"
	@echo "... src/Server/Epoll/MsgToClients.i"
	@echo "... src/Server/Epoll/MsgToClients.s"
	@echo "... src/Server/Epoll/SimpleMsgListBuilder.o"
	@echo "... src/Server/Epoll/SimpleMsgListBuilder.i"
	@echo "... src/Server/Epoll/SimpleMsgListBuilder.s"
	@echo "... src/Threads/ServerThread.o"
	@echo "... src/Threads/ServerThread.i"
	@echo "... src/Threads/ServerThread.s"
	@echo "... src/Threads/WorkerThread.o"
	@echo "... src/Threads/WorkerThread.i"
	@echo "... src/Threads/WorkerThread.s"
	@echo "... src/WaitingStrategy.o"
	@echo "... src/WaitingStrategy.i"
	@echo "... src/WaitingStrategy.s"
	@echo "... src/Worker/Worker.o"
	@echo "... src/Worker/Worker.i"
	@echo "... src/Worker/Worker.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
