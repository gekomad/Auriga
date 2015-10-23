project(auriga)

set(DR_MEMORY "NO")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -pedantic -W -DDLOG_LEVEL=INFO -DDEBUG_MODE")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -static -std=c++11 -DDLOG_LEVEL=INFO")

if (DR_MEMORY STREQUAL "YES")
    set(CMAKE_CXX_FLAGS "-DDEBUG_MODE -DDLOG_LEVEL=INFO -pthread -std=c++11 -m32 -g -fno-inline -fno-omit-frame-pointer -Wno-reorder")
endif ()

set(SOURCE_FILES

        src/engine/Engine.cpp

        src/network/Client.cpp

        src/perft/master/PerftSplitter.cpp

        src/perft/task/Perft.cpp
        src/perft/shared/PerftTree.cpp
        src/perft/task/TaskEntityDao.cpp
        src/perft/shared/PerftEntityDao.cpp

        src/perft/worker/WorkerEntityDao.cpp

        src/main.cpp)

set(CMAKE_CXX_COMPILER "g++")
add_executable(auriga ${SOURCE_FILES})
if (DR_MEMORY STREQUAL "YES")
    target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/Linux/32/libCinnamon.a)
else()
    target_link_libraries(auriga ${CMAKE_SOURCE_DIR}/src/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}/libCinnamon.a)
endif ()
