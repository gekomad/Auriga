project(auriga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wno-reorder -DDLOG_LEVEL=DEBUG -DDEBUG_MODE")

set(SOURCE_FILES
        src/pipe/Engine.cpp
        src/lib/WrapperCinnamon.h
        src/network/util/Network.h
        src/network/Client.cpp
        src/network/Client.h
        src/network/Iparser.h
        src/network/Server.cpp
        src/network/Server.h
        src/perft/shared/Message.cpp
        src/perft/shared/PerftTree.cpp
        src/perft/shared/PerftTreeDao.cpp
        src/perft/shared/Message.h
        src/perft/node/PerftClient.h
        src/perft/master/PerftSplitter.cpp
        src/perft/master/PerftSplitter.h
        src/perft/node/PerftParser.cpp
        src/perft/node/PerftParser.h
        src/perft/node/PerftResultCallback.cpp
        src/perft/node/PerftResultCallback.h
        src/perft/master/RemoteNode.cpp
        src/perft/master/RemoteNode.h
        src/main.cpp)

set(CMAKE_CXX_COMPILER "g++")
add_executable(auriga ${SOURCE_FILES})
target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/64/libCinnamon.a)