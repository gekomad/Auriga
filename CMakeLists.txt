project(auriga)

set(DR_MEMORY "NO")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -DDLOG_LEVEL=_DEBUG -DDEBUG_MODE")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -std=c++11 -DDLOG_LEVEL=_INFO")

if (DR_MEMORY STREQUAL "YES")
    set(CMAKE_CXX_FLAGS "-DDEBUG_MODE -pthread -DDLOG_LEVEL=_INFO -pthread -std=c++11 -m32 -g -fno-inline -fno-omit-frame-pointer -Wno-reorder")
endif ()

set(SOURCE_FILES

        src/engine/Engine.cpp

        src/network/Post.cpp
        src/pipe/PipePosix.cpp
#        src/pipe/PipeWindows.cpp
        src/perft/master/PerftSplitter.cpp

        src/perft/task/Perft.cpp
        src/perft/shared/PerftTree.cpp
        src/perft/task/TaskEntityDao.cpp
        src/perft/task/HttpPost.cpp
        src/perft/shared/PerftEntityDao.cpp
        src/perft/worker/WorkerEntityDao.cpp
        src/main.cpp
        src/util/Compression.h)

set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
#set(CMAKE_CXX_COMPILER "i686-w64-mingw32-g++")

add_executable(auriga ${SOURCE_FILES})

target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/64/libz.a)
if (DR_MEMORY STREQUAL "YES")
    target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/32/libCinnamon.a)
else()
    target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/64/libCinnamon.a)
endif ()

