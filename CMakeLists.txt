project(auriga)

set(DR_MEMORY "NO")


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -DDLOG_LEVEL=INFO -DDEBUG_MODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -static -std=c++11 -DDLOG_LEVEL=INFO")

if (DR_MEMORY STREQUAL "YES")
        set(CMAKE_CXX_FLAGS "-DDEBUG_MODE -DDLOG_LEVEL=INFO -pthread -std=c++11 -m32 -g -fno-inline -fno-omit-frame-pointer -Wno-reorder")
endif ()

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
        src/perft/node/Perft.cpp
        src/perft/node/PerftClient.h
        src/perft/node/PerftParser.cpp
        src/perft/node/PerftParser.h
        src/perft/node/PerftResultCallback.cpp
        src/perft/node/PerftResultCallback.h

        src/perft/shared/Message.cpp
        src/perft/shared/PerftTree.cpp
        src/perft/shared/NodeEntityDao.cpp
        src/perft/shared/PerftEntityDao.cpp

        src/util/Bits.h
        src/util/FileUtil.h
        src/util/GetOpt.h
        src/util/GetOptMaster.h
        src/util/GetOptNode.h
        src/util/Random.h
        src/util/Singleton.h

        src/util/String.h
        src/util/Time.h
        src/util/Timer.h
        src/util/UUID.h
        src/main.cpp)

set(CMAKE_CXX_COMPILER "g++")
add_executable(auriga ${SOURCE_FILES})
if (DR_MEMORY STREQUAL "YES")
        target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/32/libCinnamon.a)
else()
        target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}/libCinnamon.a)
endif ()
