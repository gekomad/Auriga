project(Auriga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wno-reorder -DDLOG_LEVEL=TRACE -DDEBUG_MODE")

set(SOURCE_FILES
    src/main.cpp
)

add_executable(Auriga ${SOURCE_FILES})

target_link_libraries(Auriga ${CMAKE_SOURCE_DIR}/lib/Linux/64/libCinnamon.so)

