project(Auriga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wno-reorder -DDLOG_LEVEL=TRACE -DDEBUG_MODE")

set(SOURCE_FILES
    src/main.cpp

        network/Server.cpp
        network/Client.cpp
        perft/remote/PerftParser.cpp

        perft/remote/RemoteNode.cpp
        perft/remote/PerftDistributed.cpp
        perft/remote/Message.cpp
        perft/remote/PerftResultCallback.cpp
)

add_executable(Auriga ${SOURCE_FILES})

target_link_libraries(Auriga ${CMAKE_SOURCE_DIR}/lib/Linux/64/libCinnamon.so)

