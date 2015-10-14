project(Auriga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wno-reorder -DDLOG_LEVEL=TRACE -DDEBUG_MODE")

set(SOURCE_FILES
    src/lib/WrapperCinnamon.h
    src/network/util/Network.h
    src/network/Client.cpp
    src/network/Client.h
    src/network/Iparser.h
    src/network/Server.cpp
    src/network/Server.h
    src/perft/Message.cpp
    src/perft/Message.h
    src/perft/PerftClient.h
    src/perft/PerftDistributed.cpp
    src/perft/PerftDistributed.h
    src/perft/PerftParser.cpp
    src/perft/PerftParser.h
    src/perft/PerftResultCallback.cpp
    src/perft/PerftResultCallback.h
    src/perft/RemoteNode.cpp
    src/perft/RemoteNode.h
    src/main.cpp)

add_executable(Auriga ${SOURCE_FILES})
target_link_libraries(Auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/64/libCinnamon.a)