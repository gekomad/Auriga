project(auriga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wno-reorder -DDLOG_LEVEL=DEBUG -DDEBUG_MODE")

set(SOURCE_FILES
    src/blockingThreadPool/ObserverThread.h
    src/blockingThreadPool/Thread.h
    src/blockingThreadPool/ThreadPool.h
    src/engine/Engine.cpp
    src/engine/Engine.h
    src/lib/WrapperCinnamon.h
    src/namespaces/debug.h
    src/namespaces/def.h
    src/network/util/Network.h
    src/network/Client.cpp
    src/network/Client.h
    src/network/Iparser.h
    src/network/Server.cpp
    src/network/Server.h
    src/perft/master/PerftSplitter.cpp
    src/perft/master/PerftSplitter.h
    src/perft/master/RemoteNode.cpp
    src/perft/master/RemoteNode.h
    src/perft/node/IResultCallback.h
    src/perft/node/Perft.h
    src/perft/node/PerftClient.h
    src/perft/node/PerftParser.cpp
    src/perft/node/PerftParser.h
    src/perft/node/PerftResultCallback.cpp
    src/perft/node/PerftResultCallback.h
    src/perft/shared/Message.cpp
    src/perft/shared/Message.h
    src/perft/shared/NodeEntity.h
    src/perft/shared/PerftEntity.h
    src/perft/shared/PerftTree.cpp
    src/perft/shared/PerftTree.h
    src/perft/shared/PerftTreeDao.cpp
    src/perft/shared/PerftTreeDao.h
    src/util/Bits.h
    src/util/FileUtil.h
    src/util/GetOpt.h
    src/util/GetOptMaster.h
    src/util/GetOptNode.h
    src/util/Random.h
    src/util/Singleton.h
    src/util/String.cpp
    src/util/String.h
    src/util/Time.h
    src/util/Timer.h
    src/util/UUID.h
    src/main.cpp)

set(CMAKE_CXX_COMPILER "g++")
add_executable(auriga ${SOURCE_FILES})
target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/64/libCinnamon.a)